#include <iostream>
#include <raylib.h>
#include <vector>
#include "constants.h"
#include <raymath.h>

using namespace std;

struct Drone {
    int id;
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    float maxSpeed = 5.0f;
    float maxForce = 0.5f;
    float wanderAngle = rand()%360;
};

void applyBoundaries(Drone& drone, float offset) {                                                                                                                                                     
    Vector2 desired = { 0.0f, 0.0f };                                                                                                                                                                  
    bool nearWall = false;

    if (drone.position.x < offset) {                                                                                                                                                                   
        desired = { drone.maxSpeed, drone.velocity.y };                                                                                                                                                
        nearWall = true;                                                                                                                                                                               
    }                                                                                                                                                                                                  
    else if (drone.position.x > GRID_SIZE_X - offset) {                                                                                                                                                
        desired = { -drone.maxSpeed, drone.velocity.y };                                                                                                                                               
        nearWall = true;                                                                                                                                                                               
    }                                                                                                                                                                                                  
                                                                                                                                                                                                           
    if (drone.position.y < offset) {
        desired = { drone.velocity.x, drone.maxSpeed };
        nearWall = true;
    } 
    else if (drone.position.y > GRID_SIZE_Y - offset) {
        desired = { drone.velocity.x, -drone.maxSpeed };
        nearWall = true;
    }
  
    if (nearWall) {
            desired = Vector2Normalize(desired);
            desired = Vector2Scale(desired, drone.maxSpeed);
  
            Vector2 steer = Vector2Subtract(desired, drone.velocity);
            steer = Vector2ClampValue(steer, 0.0f, drone.maxForce*0.75f);
  
            drone.acceleration = Vector2Add(drone.acceleration, steer);
            drone.wanderAngle = atan2f(desired.y, desired.x);
        }
    }

void updateDrones(vector<Drone>& drones, const Camera2D& camera2d) {
    for (auto& drone : drones) {

        // Wandering logic (draws circle in front of drone and chooses random targets on the circle)

        float wanderRadius = 25.0f;
        float wanderDistance = 200.0f;
        float change = 0.3f;

        float randomStep = GetRandomValue(-100, 100) / 100.0f * change;
        drone.wanderAngle += randomStep;

        Vector2 circleCenter = Vector2Normalize(drone.velocity); // Get direction
        circleCenter = Vector2Scale(circleCenter, wanderDistance); // Scale
        circleCenter = Vector2Add(drone.position, circleCenter); // Add to the drones position

        float droneHeading = atan2f(drone.velocity.y, drone.velocity.x); // Get angle of drone
        float totalAngle = droneHeading + drone.wanderAngle;

        Vector2 displacement = {wanderRadius * cosf(drone.wanderAngle), wanderRadius * sinf(drone.wanderAngle)};

        Vector2 target = Vector2Add(circleCenter, displacement);


        // Reduce speed when arriving at target (radius r)
        // float d = Vector2Length(desired);
        //float r = 300;
        //if (d < r) {
        //    float m = Remap(d, 0, r, 0, drone.maxSpeed);
        //    desired = Vector2Scale(Vector2Normalize(desired), m);
        //}
        //else {
            Vector2 desired = Vector2Scale(Vector2Normalize(desired), drone.maxSpeed);
        //}

        desired = Vector2Subtract(target, drone.position);

        Vector2 steeringForce = Vector2Subtract(desired, drone.velocity);

        steeringForce = Vector2ClampValue(steeringForce, 0.0f, drone.maxForce); // Limit the steering force
        drone.acceleration = Vector2Add(drone.acceleration, steeringForce); // apply force

        applyBoundaries(drone, 5.0f);

        drone.velocity = Vector2Add(drone.velocity, drone.acceleration);
        drone.velocity = Vector2ClampValue(drone.velocity, 0.0f, drone.maxSpeed);
        drone.position = Vector2Add(drone.position, drone.velocity);
        drone.acceleration = {0.0f, 0.0f};
    }
}

int main() {

    const int CELL_SIZE = 8;
    const int OFFSET_X = 150;
    const int OFFSET_Y = 150;
    const int SCREEN_WIDTH = GRID_SIZE_X+OFFSET_X;
    const int SCREEN_HEIGHT = GRID_SIZE_Y+OFFSET_Y;

    vector<int> map(GRID_SIZE_X * GRID_SIZE_Y);
    for (int i = 0; i < GRID_SIZE_Y; i++)
    {
        for (int k = 0; k < GRID_SIZE_X; k++)
        {
            map[i * GRID_SIZE_X + k] = rand() % 101;
        }
    }

    vector<Drone> drones;


    for (int i = 0; i < 30; i++)
    {
        drones.push_back({i, Vector2{rand()%GRID_SIZE_X, rand()%GRID_SIZE_Y}, Vector2{0,0}, Vector2{0,0}, 2.0f, 0.1f});
    }
    

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Main Window");

    int monitorCount = GetMonitorCount();
            
    int targetMonitor = 1; 
  
    if (targetMonitor < monitorCount) {
        Vector2 monitorPos = GetMonitorPosition(targetMonitor);
            
        int monitorWidth = GetMonitorWidth(targetMonitor);
        int monitorHeight = GetMonitorHeight(targetMonitor);
            
        int posX = monitorPos.x + (monitorWidth - SCREEN_WIDTH) / 2;
        int posY = monitorPos.y + (monitorHeight - SCREEN_HEIGHT) / 2;
  
        SetWindowPosition(posX, posY);
    }
  
    SetTargetFPS(60);

    Camera2D camera2d{};
    camera2d.zoom = 1.0f;
    camera2d.offset = {OFFSET_X/2, OFFSET_Y/2};

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(camera2d);

        for (int i = 0; i <= GRID_SIZE_X; i+=CELL_SIZE)
            DrawLine(i, 0, i, GRID_SIZE_Y, DARKGRAY);

        for (int i = 0; i <= GRID_SIZE_Y; i+=CELL_SIZE)
            DrawLine(0, i, GRID_SIZE_X, i, DARKGRAY);

        updateDrones(drones, camera2d);
        for(const auto& drone: drones) {
            DrawCircle(drone.position.x, drone.position.y, 5, RED);
        }

        EndMode2D();
        EndDrawing();
    }

    return 0;
}