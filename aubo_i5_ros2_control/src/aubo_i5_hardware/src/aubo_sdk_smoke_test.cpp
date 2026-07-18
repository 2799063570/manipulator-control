#include <exception>
#include <iostream>

#include <aubo_sdk/rpc.h>
#include <aubo_sdk/rtde.h>

int main()
{
  try
  {
    arcs::aubo_sdk::RpcClient rpc_client;
    arcs::aubo_sdk::RtdeClient rtde_client;
    if (rpc_client.setRequestTimeout(1000) != 0)
    {
      std::cerr << "AUBO RPC client initialization failed.\n";
      return 1;
    }
    std::cout << "AUBO SDK smoke test passed: RPC and RTDE client objects were created.\n";
    std::cout << "This test does not open a network connection or command the robot.\n";
    return 0;
  }
  catch (const std::exception & exception)
  {
    std::cerr << "AUBO SDK smoke test failed: " << exception.what() << '\n';
    return 1;
  }
}
