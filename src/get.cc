#include "get.hh"

/////////////////////////////////////////////////////////////////////////////////////////////////////
// get_member
// https://api.congress.gov/v3/member?api_key=[INSERT_KEY]&limit=250
/////////////////////////////////////////////////////////////////////////////////////////////////////

int get_member(const std::string& key, const std::string& format, int limit)
{
  const std::string host = "api.congress.gov";
  const std::string port_num = "443";
  std::stringstream http;
  http << "GET /v3/member?api_key=" << key;
  if (limit > 0)
  {
    http << "&limit=" << limit;
  }
  http << " HTTP/1.1\r\n";
  http << "Host: " << host;
  http << "\r\n";
  if (format == "json")
  {
    http << "Accept: application/json\r\n";
  }
  else if (format == "xml")
  {
    http << "Accept: application/xml\r\n";
  }
  else
  {
    http << "Accept: */*\r\n";
  }
  http << "Connection: close\r\n\r\n";
  std::cout << http.str() << std::endl;

  std::string json;
  ssl_read(host, port_num, http.str(), json);

  if (!json.size())
  {
    return -1;
  }

  std::cout << json.c_str() << std::endl;
  std::ofstream ofs("member.json");
  ofs << json;
  ofs.close();

  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// get_bill
// https://api.congress.gov/v3/bill/{congress}/{billType}?api_key=[INSERT_KEY]
// billType: hr, s, hjres, sjres, hconres, sconres, hres, sres
/////////////////////////////////////////////////////////////////////////////////////////////////////

int get_bill(const BillRequest& request)
{
  const std::string host = "api.congress.gov";
  const std::string port_num = "443";
  std::stringstream http;
  http << "GET /v3/bill/" << request.congress << "/" << request.billType
    << "?api_key=" << request.key;
  if (request.limit > 0)
  {
    http << "&limit=" << request.limit;
  }
  http << " HTTP/1.1\r\n";
  http << "Host: " << host;
  http << "\r\n";
  if (request.format == "json")
  {
    http << "Accept: application/json\r\n";
  }
  else if (request.format == "xml")
  {
    http << "Accept: application/xml\r\n";
  }
  else
  {
    http << "Accept: */*\r\n";
  }
  http << "Connection: close\r\n\r\n";
  std::cout << http.str() << std::endl;

  std::string json;
  ssl_read(host, port_num, http.str(), json);

  if (!json.size())
  {
    return -1;
  }

  std::cout << json.c_str() << std::endl;
  std::ofstream ofs("bill.json");
  ofs << json;
  ofs.close();

  return 0;
}