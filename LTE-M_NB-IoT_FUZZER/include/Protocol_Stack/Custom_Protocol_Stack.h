#pragma once

#include "Protocol_Stack/Protocol_Stack_Base.h"

/* NEEDS TO BE IMPLEMENTED BY THE USER */

class Custom_Protocol_Stack final : public Protocol_Stack_Base {
public:
    bool start();
    bool stop();
    bool restart();
    bool is_running();
};