/**
 * @file PathFinder.h
 * @brief Advanced pathfinding and navigation
 *
 * Phase 7 - Advanced Features
 *
 * Intelligent navigation with:
 * - A* pathfinding
 * - Obstacle avoidance
 * - Optimal route calculation
 * - Terrain awareness
 */

#pragma once

#include <Windows.h>
#include <vector>
#include <queue>

namespace JX1 {

/**
 * 3D Position
 */
struct Position {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Position() = default;
    Position(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    float DistanceTo(const Position& other) const;
    bool Equals(const Position& other, float epsilon = 1.0f) const;
};

/**
 * Navigation node for A* pathfinding
 */
struct NavNode {
    Position pos;
    float g = 0.0f;  // Cost from start
    float h = 0.0f;  // Heuristic to goal
    float f = 0.0f;  // Total cost (g + h)
    NavNode* parent = nullptr;

    bool operator>(const NavNode& other) const {
        return f > other.f;
    }
};

/**
 * Path - Sequence of positions
 */
struct Path {
    std::vector<Position> waypoints;
    int currentIndex = 0;
    bool completed = false;

    bool IsEmpty() const { return waypoints.empty(); }
    Position GetCurrent() const;
    Position GetNext();
    bool HasNext() const { return currentIndex < (int)waypoints.size(); }
    void Reset() { currentIndex = 0; completed = false; }
};

/**
 * PathFinder - Advanced navigation system
 *
 * Usage:
 *   PathFinder* pf = PathFinder::Get();
 *   Path path = pf->FindPath(startPos, endPos);
 *
 *   while (path.HasNext()) {
 *       Position next = path.GetNext();
 *       MoveTo(next.x, next.y, next.z);
 *   }
 */
class PathFinder {
public:
    /**
     * Get singleton instance
     */
    static PathFinder* Get();

    /**
     * Initialize pathfinder
     */
    bool Initialize();

    // ========================================
    // Pathfinding
    // ========================================

    /**
     * Find path from start to end using A*
     * @return Path object with waypoints
     */
    Path FindPath(const Position& start, const Position& end);

    /**
     * Find path with max distance limit
     */
    Path FindPath(const Position& start, const Position& end, float maxDistance);

    /**
     * Check if path is clear (no obstacles)
     */
    bool IsPathClear(const Position& start, const Position& end);

    /**
     * Check if position is walkable
     */
    bool IsWalkable(const Position& pos);

    // ========================================
    // Navigation Helpers
    // ========================================

    /**
     * Get closest walkable position to target
     */
    Position GetClosestWalkable(const Position& target);

    /**
     * Calculate distance between positions
     */
    float GetDistance(const Position& a, const Position& b);

    /**
     * Smooth path (remove unnecessary waypoints)
     */
    void SmoothPath(Path& path);

    // ========================================
    // Obstacle Detection
    // ========================================

    /**
     * Check for obstacles in area
     */
    bool HasObstacle(const Position& pos, float radius = 10.0f);

    /**
     * Find path around obstacle
     */
    Position FindAvoidancePoint(const Position& current, const Position& goal);

private:
    PathFinder() = default;
    ~PathFinder() = default;
    PathFinder(const PathFinder&) = delete;
    PathFinder& operator=(const PathFinder&) = delete;

    /**
     * A* algorithm implementation
     */
    std::vector<Position> AStar(const Position& start, const Position& goal);

    /**
     * Heuristic function (Manhattan distance)
     */
    float Heuristic(const Position& a, const Position& b);

    /**
     * Get neighbors for A* expansion
     */
    std::vector<Position> GetNeighbors(const Position& pos);

    /**
     * Reconstruct path from A* result
     */
    std::vector<Position> ReconstructPath(NavNode* endNode);

    bool m_initialized = false;
};

} // namespace JX1
