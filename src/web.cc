#include "web.hh"
#include "get.hh"

/////////////////////////////////////////////////////////////////////////////////////////////////////
// ApplicationCongress
/////////////////////////////////////////////////////////////////////////////////////////////////////

ApplicationCongress::ApplicationCongress(const Wt::WEnvironment& env)
  : WApplication(env)
{
  Wt::WContainerWidget* input = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
  useStyleSheet("https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css");
  input->addWidget(std::make_unique<Wt::WText>("API Key: "));
  edit_key = input->addWidget(std::make_unique<Wt::WLineEdit>());
  edit_key->setWidth(Wt::WLength(400));

  Wt::WPushButton* load_button = input->addWidget(std::make_unique<Wt::WPushButton>("Load Members"));
  load_button->clicked().connect(this, &ApplicationCongress::on_load_button_clicked);

  Wt::WBreak* br = root()->addWidget(std::make_unique<Wt::WBreak>());
  container = root()->addWidget(std::make_unique<Wt::WContainerWidget>());

  std::vector<Member> members;
  if (load_member("member.json", members) == 0)
  {
    display_members(members);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// ~ApplicationCongress
/////////////////////////////////////////////////////////////////////////////////////////////////////

ApplicationCongress::~ApplicationCongress()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// on_load_button_clicked
/////////////////////////////////////////////////////////////////////////////////////////////////////

void ApplicationCongress::on_load_button_clicked()
{
  std::string key = edit_key->text().toUTF8();

  if (key.empty())
  {
    container->clear();
    container->addWidget(std::make_unique<Wt::WText>("Please enter an API key"));
    return;
  }

  if (get_member(key, "json", 2) < 0)
  {
    container->clear();
    return;
  }

  std::vector<Member> members;
  if (load_member("member.json", members) == 0)
  {
    container->clear();
    display_members(members);
  }
  else
  {
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// create_application
/////////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<Wt::WApplication> create_application(const Wt::WEnvironment& env)
{
  return std::make_unique<ApplicationCongress>(env);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// main
/////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
  return Wt::WRun(argc, argv, &create_application);
}
