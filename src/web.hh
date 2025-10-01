#ifndef WT_WEB_HH_
#define WT_WEB_HH_

#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <Wt/WTable.h>
#include <Wt/WTableCell.h>
#include <Wt/WImage.h>
#include <Wt/WBreak.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WCssDecorationStyle.h>
#include <Wt/Json/Value.h>
#include <Wt/Json/Object.h>
#include <Wt/Json/Array.h>
#include <Wt/Json/Parser.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Member structure
/////////////////////////////////////////////////////////////////////////////////////////////////////

struct Term
{
  std::string chamber;
  int startYear = 0;
  int endYear = 0;
};

struct Member
{
  std::string name;
  std::string partyName;
  std::string state;
  std::string imageUrl;
  std::vector<Term> terms;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
// ApplicationCongress
/////////////////////////////////////////////////////////////////////////////////////////////////////

class ApplicationCongress : public Wt::WApplication
{
public:
  ApplicationCongress(const Wt::WEnvironment& env);
  virtual ~ApplicationCongress();

private:
  int load_member(const std::string& buf, std::vector<Member>& members);
  void display_members(const std::vector<Member>& members);
  std::string format_terms(const Member& member);
  void on_load_button_clicked();
  Wt::WLineEdit* edit_key;
  Wt::WContainerWidget* container;
};

#endif