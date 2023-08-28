#include <iostream>

#include "app_config.h"

int main(int argc, char *argv[]) {
  AppConfig config;

  if (!config.parse(argc, argv)) {
    return 1;
  }
  
}