#ifndef UI_HPP
#define UI_HPP

#include <string>

std::string getAssetsPath(); // 👈 ADD THIS

void print_logo(const std::string& assetsPath);
void print_menu(const std::string& assetsPath);

#endif // UI_HPP
