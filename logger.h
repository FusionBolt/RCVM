#pragma once
#include <glog/logging.h>

namespace RCVM {
    struct LoggerStatus {
        bool inst = true;
        bool state = true;
        bool exec = true;
        bool stack = false;
    };

    void init_logger(char* argv[])
    {
        google::InitGoogleLogging(argv[0]);
        FLAGS_logtostderr = true;
        FLAGS_alsologtostderr = false;
        FLAGS_colorlogtostderr = true;
    }
    inline LoggerStatus logger_status;
}

#define DEBUG
// todo:log option for enable dump pc, stack pointer...
#ifdef DEBUG
#define LOG_DEBUG(info) LOG(INFO) << info
#define INST_LOG(inst_str) LOG_IF(INFO, RCVM::logger_status.inst) << "Current Inst: " << inst_str
#define STATE_LOG(msg) LOG_IF(INFO, RCVM::logger_status.state) << msg
#define EXEC_LOG(msg) LOG_IF(INFO, RCVM::logger_status.exec) << msg
#define STACK_LOG(msg) LOG_IF(INFO, RCVM::logger_status.stack) << msg
#else
#define LOG_DEBUG(info)
#define INST_LOG(inst)
#define STATES_LOG(msg)
#define EXEC_LOG(msg)
#endif