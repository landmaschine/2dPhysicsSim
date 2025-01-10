#version 430

struct GpuCollisionData {
    float x;
    float y;
    float r;
};

layout(std430, binding = 0) buffer CollisionInput {
    GpuCollisionData data[];
} inputData;

layout(std430, binding = 1) buffer CollisionOutput {
    GpuCollisionData data[];
} outputData;

layout(local_size_x = 1024) in;

uniform float responseCoef;
uniform int particleCount;
uniform vec2 world_size;

void resolveCollision(inout vec2 posA, float radiusA, vec2 posB, float radiusB) {
    vec2 v = posA - posB;
    float min_dist = radiusA + radiusB;
    float dist2 = v.x * v.x + v.y * v.y;

    if(dist2 < min_dist * min_dist) {
        float dist = sqrt(dist2);
        vec2 n = v / dist;
        float mass_ratio_1 = radiusA / (radiusA + radiusB);
        float mass_ratio_2 = radiusB / (radiusA + radiusB);
        float delta = 0.5 * responseCoef * (dist - min_dist);

        posA -= n * (mass_ratio_2 * delta);
    }
}

void clampToBounds(inout vec2 position, float radius, vec2 worldSize) {
    const int WALL_SEGMENTS = 3;

    for(int i = 0; i < WALL_SEGMENTS; i++) {
        float t = float(i) / float(WALL_SEGMENTS - 1);

        vec2 leftPos = vec2(0.0, mix(position.y - radius, position.y + radius, t));
        resolveCollision(position, radius, leftPos, radius);

        vec2 rightPos = vec2(worldSize.x, mix(position.y - radius, position.y + radius, t));
        resolveCollision(position, radius, rightPos, radius);

        vec2 topPos = vec2(mix(position.x - radius, position.x + radius, t), 0.0);
        resolveCollision(position, radius, topPos, radius);

        vec2 bottomPos = vec2(mix(position.x - radius, position.x + radius, t), worldSize.y);
        resolveCollision(position, radius, bottomPos, radius);
    }

    position = clamp(position, 
                    vec2(radius, radius), 
                    vec2(worldSize.x - radius, worldSize.y - radius));
}

void main() {
    uint id = gl_GlobalInvocationID.x;
    if (id >= particleCount) return;

    GpuCollisionData particle = inputData.data[id];
    vec2 position = vec2(particle.x, particle.y);
    float radius = particle.r;

    for(int i = 0; i < particleCount; i++) {
        if(i != id) {
            GpuCollisionData other = inputData.data[i];
            vec2 otherPos = vec2(other.x, other.y);
            
            resolveCollision(position, radius, otherPos, other.r);
        }
    }

    clampToBounds(position, radius, world_size);

    particle.x = position.x;
    particle.y = position.y;
    outputData.data[id] = particle;
}