#include "process_nvram.h"

std::string gTestData =
    "<ValidName>11:22:33:44:55:66>0>0>>>>"
    "<OtherValidName>22:23:24:25:26:27>0>0>>>>"
    "<ConflictingName>11:12:13:14:15:16>0>0>>>>"
    "<ConflictingName>11:12:13:14:15:17>0>0>>>>"
    "<InvalidName:BadCharacter>00:11:22:33:44:55>0>6>>>>"
    "<InvalidName Spaces>00:11:22:33:44:56>0>6>>>>"
    "<>00:11:22:33:44:55>0>4>>";

int main(void) {
  auto map = asus::ProcessCustomClientList(gTestData);
}
