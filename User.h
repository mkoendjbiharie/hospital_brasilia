#ifndef USER_H_
#define USER_H_

#include <Wt/Dbo/Types>
#include <Wt/WGlobal>

namespace dbo = Wt::Dbo;

class User;
typedef Wt::Auth::Dbo::AuthInfo<User> AuthInfo;

class User {
public:
  /* You probably want to add other user information here */

  template<class Action>
  void persist(Action& a)
  {
  }
};



DBO_EXTERN_TEMPLATES(User);

#endif // USER_H_