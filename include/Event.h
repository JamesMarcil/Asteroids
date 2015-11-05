#pragma once
#include <string>
#include <cstdint>

using namespace std;

struct Event
{
	string name;
	uintptr_t data;
};
