#pragma once

#include<iostream>
#include <string>
#include <vector>
#include "ActionHandler.h"
#include "../Actions/ActionType.h"
#include "../Actions/Action.h"

class OrderActionHandler : public ActionHandler {
 public:
  OrderActionHandler() = default;
  OrderActionHandler(StockExchange* se);
  std::vector<std::string> HandleAction(Action action);
};