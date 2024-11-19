#include "sim.h"
#include <math.h>
#include <string.h>

struct node {
    struct node *next;
    size_t index;
};

static struct node nodes[N_BALLS];
static struct node *tiles[16][16][16];

static int min(int a, int b) {
    return a < b ? a : b;
}

static int max(int a, int b) {
    return a > b ? a : b;
}

void init_sim(void) {
    for (int i = 0; i < N_BALLS; i++) {
        balls_pos[i][0] = i % 16 + 0.5f;
        balls_pos[i][1] = i / 16 % 16 + 0.5f;
        balls_pos[i][2] = i / 256 % 16 + 0.5f;
    }
    for (int i = 0; i < N_BALLS; i++) {
        balls_vel[i][0] = 2.0 * drand48() - 1.0;
        balls_vel[i][1] = 2.0 * drand48() - 1.0;
        balls_vel[i][2] = 2.0 * drand48() - 1.0;
    }
}

void update_sim(void) {
    memset(tiles, 0, sizeof(tiles));
    for (int i = 0; i < N_BALLS; i++) {
        int x = balls_pos[i][0];
        int y = balls_pos[i][1];
        int z = balls_pos[i][2];
        nodes[i].next = tiles[x][y][z];
        nodes[i].index = i;
        tiles[x][y][z] = &nodes[i];
    }
    for (int i = 0; i < N_BALLS; i++) {
        glm_vec3_muladds(balls_vel[i], DT, balls_pos[i]); 
    }
    for (int i = 0; i < N_BALLS; i++) {
        for (int j = 0; j < 3; j++) {
            if (balls_pos[i][j] < RADIUS) {
                balls_pos[i][j] = RADIUS;
                balls_vel[i][j] = -balls_vel[i][j];
            } else if (balls_pos[i][j] > 16.0f - RADIUS) {
                balls_pos[i][j] = 16.0f - RADIUS;
                balls_vel[i][j] = -balls_vel[i][j];
            }
        }
        int bx = balls_pos[i][0];
        int by = balls_pos[i][1];
        int bz = balls_pos[i][2];
        int x0 = max(bx - 1, 0); 
        int y0 = max(by - 1, 0);
        int z0 = max(bz - 1, 0);
        int x1 = min(bx + 1, 15); 
        int y1 = min(by + 1, 15);
        int z1 = min(bz + 1, 15);
        for (int x = x0; x <= x1; x++) {
            for (int y = y0; y <= y1; y++) {
                for (int z = z0; z <= z1; z++) {
                    for (struct node *n = tiles[x][y][z]; n; n = n->next) {
                        int j = n->index;
                        if (j >= i)
                            continue;
                        vec3 normal;
                        glm_vec3_sub(balls_pos[i], balls_pos[j], normal);
                        float d2 = glm_vec3_norm2(normal); 
                        if (d2 > 0.0f && d2 < DIAMETER * DIAMETER) {
                            float d = sqrtf(d2);
                            glm_vec3_divs(normal, d, normal);
                            float corr = (DIAMETER - d) / 2.0f;
                            glm_vec3_muladds(normal, corr, balls_pos[i]);
                            glm_vec3_mulsubs(normal, corr, balls_pos[j]);
                            float vi = glm_vec3_dot(balls_vel[i], normal);
                            float vj = glm_vec3_dot(balls_vel[j], normal);
                            glm_vec3_muladds(normal, vj - vi, balls_vel[i]); 
                            glm_vec3_muladds(normal, vi - vj, balls_vel[j]); 
                        }
                    }
                }
            }
        }
    }
}
