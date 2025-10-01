#include "web.hh"

/////////////////////////////////////////////////////////////////////////////////////////////////////
// load_member
/////////////////////////////////////////////////////////////////////////////////////////////////////

int ApplicationCongress::load_member(const std::string& buf, std::vector<Member>& members)
{
  try
  {
    std::ifstream ifs(buf);
    if (!ifs.is_open())
    {
      return -1;
    }

    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string json_buf = ss.str();

    Wt::Json::ParseError error;
    Wt::Json::Value result;

    if (!Wt::Json::parse(json_buf, result, error))
    {
      return -1;
    }

    Wt::Json::Object& root = result;
    const Wt::Json::Array& members_array = root.get("members");

    members.clear();

    for (size_t idx = 0; idx < members_array.size(); ++idx)
    {
      const Wt::Json::Object& obj = members_array[idx];

      Member member;

      if (!obj.get("name").isNull())
      {
        Wt::WString name = obj.get("name");
        member.name = name.toUTF8();
      }

      if (!obj.get("partyName").isNull())
      {
        Wt::WString partyName = obj.get("partyName");
        member.partyName = partyName.toUTF8();
      }

      if (!obj.get("state").isNull())
      {
        Wt::WString state = obj.get("state");
        member.state = state.toUTF8();
      }

      if (!obj.get("depiction").isNull())
      {
        const Wt::Json::Object& depiction = obj.get("depiction");
        if (!depiction.get("imageUrl").isNull())
        {
          Wt::WString imageUrl = depiction.get("imageUrl");
          member.imageUrl = imageUrl.toUTF8();
        }
      }

      if (!obj.get("terms").isNull() && obj.get("terms").type() == Wt::Json::Type::Object)
      {
        const Wt::Json::Object& terms_obj = obj.get("terms");
        Wt::Json::Object::const_iterator item_it = terms_obj.find("item");

        if (item_it != terms_obj.end() && item_it->second.type() == Wt::Json::Type::Array)
        {
          const Wt::Json::Array& items = item_it->second;

          for (size_t i = 0; i < items.size(); ++i)
          {
            const Wt::Json::Value& value = items[i];

            if (value.type() == Wt::Json::Type::Object)
            {
              const Wt::Json::Object& term = value;
              Term t;
              Wt::Json::Object::const_iterator it;

              it = term.find("chamber");
              if (it != term.end() && it->second.type() == Wt::Json::Type::String)
              {
                t.chamber = std::string(it->second);
              }

              it = term.find("startYear");
              if (it != term.end() && it->second.type() == Wt::Json::Type::Number)
              {
                t.startYear = static_cast<int>(it->second.toNumber());
              }

              it = term.find("endYear");
              if (it != term.end() && it->second.type() == Wt::Json::Type::Number)
              {
                t.endYear = static_cast<int>(it->second.toNumber());
              }

              member.terms.push_back(t);
            }
          }
        }
      }

      members.push_back(member);
    }
  }
  catch (const std::exception& e)
  {
    return -1;
  }

  return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
// display_members
/////////////////////////////////////////////////////////////////////////////////////////////////////

void ApplicationCongress::display_members(const std::vector<Member>& members)
{
  if (members.empty())
  {
    return;
  }

  std::string count_text = "Showing " + std::to_string(members.size()) + " members";
  Wt::WText* count_info = container->addNew<Wt::WText>(count_text);
  count_info->setStyleClass("text-muted mb-3");

  Wt::WTable* table = container->addNew<Wt::WTable>();
  table->setStyleClass("table table-striped table-bordered");
  table->setHeaderCount(1);

  table->elementAt(0, 0)->addNew<Wt::WText>("Photo");
  table->elementAt(0, 1)->addNew<Wt::WText>("Name");
  table->elementAt(0, 2)->addNew<Wt::WText>("Party");
  table->elementAt(0, 3)->addNew<Wt::WText>("State");
  table->elementAt(0, 4)->addNew<Wt::WText>("Terms of Service");

  for (int col = 0; col < 5; ++col)
  {
    table->elementAt(0, col)->setStyleClass("table-dark");
  }


  for (size_t idx = 0; idx < members.size(); ++idx)
  {
    const Member& member = members[idx];
    int row = idx + 1;

    if (!member.imageUrl.empty())
    {
      Wt::WImage* img = table->elementAt(row, 0)->addNew<Wt::WImage>(Wt::WLink(member.imageUrl));
      img->setWidth(50);
      img->setHeight(50);
      img->setStyleClass("rounded-circle");
    }
    else
    {
      table->elementAt(row, 0)->addNew<Wt::WText>("No image");
    }

    std::string name = !member.name.empty() ? member.name : "N/A";
    table->elementAt(row, 1)->addNew<Wt::WText>(name);

    std::string party = !member.partyName.empty() ? member.partyName : "N/A";
    Wt::WText* party_text = table->elementAt(row, 2)->addNew<Wt::WText>(party);
    if (party == "Republican")
    {
      party_text->setStyleClass("badge badge-danger");
    }
    else if (party == "Democratic")
    {
      party_text->setStyleClass("badge badge-primary");
    }
    else
    {
      party_text->setStyleClass("badge badge-secondary");
    }

    std::string state = !member.state.empty() ? member.state : "N/A";
    table->elementAt(row, 3)->addNew<Wt::WText>(state);

    std::string terms = format_terms(member);
    table->elementAt(row, 4)->addNew<Wt::WText>(terms);
  }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
// format_terms
/////////////////////////////////////////////////////////////////////////////////////////////////////

std::string ApplicationCongress::format_terms(const Member& member)
{
  if (member.terms.empty())
  {
    return "N/A";
  }

  std::vector<std::string> terms;

  for (std::vector<Term>::const_iterator it = member.terms.begin(); it != member.terms.end(); ++it)
  {
    const Term& term = *it;

    std::string chamber = term.chamber;
    if (chamber == "House of Representatives")
    {
      chamber = "House";
    }
    else if (chamber.empty())
    {
      chamber = "Unknown";
    }

    std::string start_year = (term.startYear > 0) ? std::to_string(term.startYear) : "?";
    std::string end_year = (term.endYear > 0) ? "-" + std::to_string(term.endYear) : "-Present";
    std::string buf = chamber + " (" + start_year + end_year + ")";
    terms.push_back(buf);
  }

  std::string result;
  for (size_t idx = 0; idx < terms.size(); ++idx)
  {
    if (idx > 0)
    {
      result += ", ";
    }
    result += terms[idx];
  }

  return result;
}
