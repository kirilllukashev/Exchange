#include "StockExchange.hpp"

StockExchange::StockExchange(std::string name) {
  std::transform(name.begin(), name.end(), name.begin(), ::toupper);
  this->name_ = name;
  traders_.resize(0);
  companies_.resize(0);
  orders_.resize(0);
}

bool StockExchange::AddTrader(Trader t) {
  if (GetTraderInd(t.GetId()) == -1) {
    traders_.push_back(t);
    return true;
  }
  return false;
}

std::string StockExchange::DeleteTrader(long id) {
  int ind = GetTraderInd(id);
  if (ind != -1) {
    return DeleteTrader(ind);
  }
  return ("Trader with id " + std::to_string(id) + " isn't registered with " + GetName());
}

std::string StockExchange::DeleteTrader(int i) {
  for (std::vector<Trader>::iterator it = traders_.begin(); it != traders_.end(); ++it) {
    if (it->GetId() == traders_[i].GetId()) {
      std::string ans = ("Deleted " + traders_[i].ToString());
      traders_.erase(it);
      return ans;
    }
  }
  return ("Failed to delete trader " + std::to_string(traders_[i].GetId()));
}

std::string StockExchange::GetName() {
  return name_;
}

std::string StockExchange::CheckTrader(long id) {
  for (Trader t : traders_) {
    if (t.GetId() == id) {
      return t.ToString();
    }
  }
  return ("Trader with id " + std::to_string(id) + " isn't registered");
}

int StockExchange::GetTraderInd(long id)  {
  for (int i = 0; i < traders_.size(); ++i) {
    if (traders_[i].GetId() == id) {
      return i;
    }
  }
  return -1;
}

Trader StockExchange::GetTraderByIndex(int index) {
  return traders_[index];
}
std::vector<Company> StockExchange::GetCompanies() {
  return companies_;
}

int StockExchange::GetCompanyIndex(std::string ticker) {
  std::transform(ticker.begin(), ticker.end(), ticker.begin(), ::toupper);
  for (int i = 0; i < companies_.size(); ++i) {
    std::string tic = companies_[i].GetStock().GetTicker();
    if (companies_[i].GetStock().GetTicker() == ticker) {
      return i;
    }
  }
  return -1;
}

Company StockExchange::GetCompanyByIndex(int index) {
  return companies_[index];
}

std::vector<Company> StockExchange::GetCompaniesByCategory(std::string ticker) {
  std::vector<Company> return_ans;
  for (Company c : companies_) {
    if (c.GetCategory() == ticker) {
      return_ans.push_back(c);
    }
  }
  return return_ans;
}

bool StockExchange::AddCompany(Company c) {
  if (GetCompanyIndex(c.GetStock().GetTicker()) == -1) {
    this->companies_.push_back(c);
    return true;
  }
  return false;
}

std::string StockExchange::DeleteCompany(std::string ticker) {
  int ind = GetCompanyIndex(ticker);
  if (ind != -1) {
    std::string ans = companies_[ind].ToString();
    companies_.erase(companies_.begin() + ind);
    return ans;
  }
  return "NULL";
}

template<typename T>
std::string StockExchange::ToString(std::vector<T> vector) {
  std::string ans = "{";
  for (auto x : vector) {
    ans += x.ToString();
  }
  ans += "}";
  return ans;
}

void StockExchange::AddOrder(Order o) {
  orders_.push_back(o);
}

std::vector<Order> StockExchange::GetOrders() {
  return orders_;
}

std::string StockExchange::ExecuteOrder(Order o, int index_order) {
  Stock s = o.GetStock();
  std::string ticker = s.GetTicker();
  int index_company = GetCompanyIndex(ticker);
  int index_trader = GetTraderInd(o.GetTrader().GetId());
  Company c = GetCompanyByIndex(index_company);
  if ((s.GetLowPrice() <= o.GetRate() && o.GetRate() <= s.GetHighPrice())
      && c.GetQuantity() >= o.GetQuantity()
      && o.GetQuantity() * o.GetRate() <= o.GetTrader().GetCurrency()) {
    companies_[index_company].SetQuantity(companies_[index_company].GetQuantity() - o.GetQuantity());
    int actual_quan = traders_[index_trader].GetHolding(o.GetStock());
    traders_[index_trader].PutHolding(o.GetStock(), actual_quan + o.GetQuantity());
    traders_[index_trader].SetCurrency(traders_[index_trader].GetCurrency() - o.GetQuantity() * o.GetRate());
    orders_.erase(orders_.begin() + index_order);
    return o.ToString() + "Accepted!";
  } else {
    return o.ToString() + "Failed!";
  }
}

std::string StockExchange::ToString() {
  std::string return_ans =  "StockExchange {name='" + name_ + '\'' +
                            ", publiclyListedCompanies=" + ToString(companies_) +
                            ", orders=" + ToString(orders_) + '}';
  return return_ans;
}
