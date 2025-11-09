#ifndef PCI_DEVICE_ENUMERATOR_H
#define PCI_DEVICE_ENUMERATOR_H

#include <vector>
#include <string>
#include <tuple>

std::vector<std::tuple<std::string, std::string, std::string>> EnumeratePCIDevices();

#endif // PCI_DEVICE_ENUMERATOR_H