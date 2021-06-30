#include <iostream>
#include <fstream>
#include <string>

#include "ns3/core-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Logger");

int
main (int argc, char *argv[])
{

  // Siulator::Run() and Simulator::Destroy () are used to ensure that the NS_LOG macros print timestamp and context.
  Simulator::Run ();
  NS_LOG_DEBUG ("Debug");
  NS_LOG_INFO ("Info");
  NS_LOG_WARN ("Warn");
  NS_LOG_ERROR ("Error");
  NS_LOG_LOGIC ("Logic");
  NS_LOG_FUNCTION ("Function");
  Simulator::Destroy ();
}
