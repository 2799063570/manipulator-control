/** @file  sync_move.h
 *  @brief 同步运行
 *
 * 1. Independent movements
 *   If the different task programs, and their robots, work independently, no
 *   synchronization or coordination is needed. Each task program is then
 *   written as if it was the program for a single robot system.
 *
 * 2. Semi coordinated movements
 *   Several robots can work with the same work object, without synchronized
 *   movements, as long as the work object is not moving.
 *   A positioner can move the work object when the robots are not coordinated
 *   to it, and the robots can be coordinated to the work object when it is not
 *   moving. Switching between moving the object and coordinating the robots is
 *   called semi coordinated movements.
 *
 * 3. Coordinated synchronized movements
 *   Several robots can work with the same moving work object.
 *   The positioner or robot that holds the work object and the robots that work
 *   with the work object must have synchronized movements. This means that the
 *   RAPID task programs, that handle one mechanical unit each, execute their
 *   move instructions simultaneously.
 */
#ifndef AUBO_SDK_SYNC_MOVE_INTERFACE_H
#define AUBO_SDK_SYNC_MOVE_INTERFACE_H

#include <vector>
#include <unordered_set>
#include <string>
#include <memory>
#include <aubo/global_config.h>

namespace arcs {
namespace common_interface {

typedef std::unordered_set<std::string> TaskSet;
class ARCS_ABI_EXPORT SyncMove
{
public:
    SyncMove();
    virtual ~SyncMove();

    /**
     *
     * @return
     *
     * @par Python函数原型
     * sync(self: pyaubo_sdk.SyncMove) -> int
     *
     * @par Lua函数原型
     * sync() -> nil
     *
     */
    int sync();

    /**
     * syncMoveOn is used to start synchronized movement mode.
     *
     * A syncMoveOn instruction will wait for the other task programs. When
     * all task programs have reached the syncMoveOn, they will continue
     * their execution in synchronized movement mode. The move instructions
     * in the different task programs are executed simultaneously, until the
     * instruction syncMoveOff is executed.
     * A stop point must be programmed before the syncMoveOn instruction.
     *
     * @param syncident
     * @param taskset
     * @return
     *
     * @par Python函数原型
     * syncMoveOn(self: pyaubo_sdk.SyncMove, arg0: str, arg1: Set[str]) -> int
     *
     * @par Lua函数原型
     * syncMoveOn(syncident: string, taskset: table) -> nil
     * @endcoe
     */
    int syncMoveOn(const std::string &syncident, const TaskSet &taskset);

    /**
     * 设置同步路径段的ID
     * In synchronized movements mode, all or none of the simultaneous move
     * instructions must be programmed with corner zones. This means that the
     * move instructions with the same ID must either all have corner zones, or
     * all have stop points. If a move instruction with a corner zone and a move
     * instruction with a stop point are synchronously executed in their
     * respective task program, an error will occur.
     *
     * Synchronously executed move instructions can have corner zones of
     * different sizes (e.g. one use z10 and one use z50).
     *
     * @param id
     * @return
     *
     * @par Python函数原型
     * syncMoveSegment(self: pyaubo_sdk.SyncMove, arg0: int) -> bool
     *
     * @par Lua函数原型
     * syncMoveSegment(id: number) -> boolean
     * @endcoe
     */
    bool syncMoveSegment(int id);

    /**
     * syncMoveOff is used to end synchronized movement mode.
     *
     * A syncMoveOff instruction will wait for the other task programs. When
     * all task programs have reached the syncMoveOff, they will continue
     * their execution in unsynchronized mode.
     * A stop point must be programmed before the syncMoveOff instruction.
     *
     * @param syncident
     * @return
     *
     * @par Python函数原型
     * syncMoveOff(self: pyaubo_sdk.SyncMove, arg0: str) -> int
     *
     * @par Lua函数原型
     * syncMoveOff(syncident: string) -> nil
     * @endcoe
     */
    int syncMoveOff(const std::string &syncident);

    /**
     * syncMoveUndo is used to turn off synchronized movements, even if not
     * all the other task programs execute the syncMoveUndo instruction.
     *
     * syncMoveUndo is intended for UNDO handlers. When the program
     * pointer is moved from the procedure, syncMoveUndo is used to turn off
     * the synchronization.
     *
     * @return
     *
     * @par Python函数原型
     * syncMoveUndo(self: pyaubo_sdk.SyncMove) -> int
     *
     * @par Lua函数原型
     * syncMoveUndo() -> nil
     * @endcoe
     */
    int syncMoveUndo();

    /**
     * waitSyncTasks used to synchronize several task programs at a special
     * point in the program.
     *
     * A waitSyncTasks instruction will wait for the other
     * task programs. When all task programs have reached the waitSyncTasks
     * instruction, they will continue their execution.
     *
     * @param syncident
     * @param taskset
     * @return
     *
     * @par Python函数原型
     * waitSyncTasks(self: pyaubo_sdk.SyncMove, arg0: str, arg1: Set[str]) ->
     * int
     *
     * @par Lua函数原型
     * waitSyncTasks(syncident: string, taskset: table) -> nil
     * @endcoe
     */
    int waitSyncTasks(const std::string &syncident, const TaskSet &taskset);

    /**
     * isSyncMoveOn is used to tell if the mechanical unit group is in synchron-
     * ized movement mode.
     *
     * A task that does not control any mechanical unit can find out if the
     * mechanical units defined in the parameter Use Mechanical Unit Group are
     * in synchronized movement mode.
     *
     * @return
     *
     * @par Python函数原型
     * isSyncMoveOn(self: pyaubo_sdk.SyncMove) -> bool
     *
     * @par Lua函数原型
     * isSyncMoveOn() -> boolean
     * @endcoe
     */
    bool isSyncMoveOn();

    /**
     *
     * @return
     *
     * @par Python函数原型
     * syncMoveSuspend(self: pyaubo_sdk.SyncMove) -> int
     *
     * @par Lua函数原型
     * syncMoveSuspend() -> nil
     * @endcoe
     */
    int syncMoveSuspend();

    /**
     *
     * @return
     *
     * @par Python函数原型
     * syncMoveResume(self: pyaubo_sdk.SyncMove) -> int
     *
     * @par Lua函数原型
     * syncMoveResume() -> nil
     * @endcoe
     */
    int syncMoveResume();

protected:
    void *d_;
};

using SyncMovePtr = std::shared_ptr<SyncMove>;

// clang-format off
#define SyncMove_DECLARES                                  \
    _INST(SyncMove, 0, sync)                               \
    _INST(SyncMove, 2, syncMoveOn, syncident, taskset)     \
    _INST(SyncMove, 1, syncMoveSegment, id)                \
    _INST(SyncMove, 1, syncMoveOff, syncident)             \
    _INST(SyncMove, 0, syncMoveUndo)                       \
    _INST(SyncMove, 2, waitSyncTasks, syncident, taskset)  \
    _FUNC(SyncMove, 0, isSyncMoveOn)                       \
    _INST(SyncMove, 0, syncMoveSuspend)                    \
    _INST(SyncMove, 0, syncMoveResume)
// clang-format on
} // namespace common_interface
} // namespace arcs
#endif // AUBO_SDK_SYNC_MOVE_INTERFACE_H
