/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2026    Equinor ASA
//
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
//
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Thread-safe global queue enforcing the configured external-process concurrency limit.
#pragma once

#include <list>

#include <QMutex>

class RimProcess;

/// @brief Coordinates waiting and running RimProcess objects without owning them.
///
/// The singleton starts jobs in FIFO order until the OPM preference concurrency limit is reached.
/// Callers retain ownership of every process and must keep it alive while queued or running.
/// Queue mutations are serialized by m_mutex; stop and completion callbacks also trigger the next
/// eligible launch.
class RimProcessQueue
{
public:
    /// Adds @p process to the queue and returns its identifier, or zero for null.
    static size_t queueProcess( RimProcess* process );
    /// Removes a waiting process or kills a running process with @p processId.
    static void   stopProcess( size_t processId );
    /// Releases the queue slot associated with a completed process.
    static void   onProcessFinished( size_t processId );

protected:
    /// Constructs the singleton's initially empty queues.
    RimProcessQueue();
    /// @return Function-local singleton queue instance.
    static RimProcessQueue* instance();

    /// Locked implementation of queueProcess().
    size_t internalQueueProcess( RimProcess* process );
    /// Locked implementation of onProcessFinished().
    void   internalOnProcessFinished( size_t processId );
    /// Removes @p processId under the mutex, then notifies or terminates it outside the lock.
    void   internalStopProcess( size_t processId );

private:
    /// Starts the next FIFO job when both queue capacity and work are available.
    void launchNextProcessIfPossible();

    /// Non-owning FIFO of processes not yet started.
    std::list<RimProcess*> m_waitingProcesses;
    /// Non-owning collection of processes occupying concurrency slots.
    std::list<RimProcess*> m_runningProcesses;

    /// Protects both process lists and launch decisions.
    QMutex m_mutex;
};
