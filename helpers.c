/**
  * Created by max on 11.04.19.
  */

#include "helpers.h"

const char *get_user_name() {
  uid_t uid = geteuid();
  struct passwd *pw = getpwuid(uid);
  if (pw) {
    return pw->pw_name;
  }
  return "";
}

char *get_time() {
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  return asctime(tm);
}
