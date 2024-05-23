﻿using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace LunaSharp
{
    public static class EntryPoint
    {
        private static IntPtr HModule { get; set; } = IntPtr.Zero;
        private const uint DLL_PROCESS_DETACH = 0,
                           DLL_PROCESS_ATTACH = 1,
                           DLL_THREAD_ATTACH = 2,
                           DLL_THREAD_DETACH = 3;



        [UnmanagedCallersOnly(EntryPoint = "DllMain", CallConvs = new[] { typeof(CallConvStdcall) })]
        public static bool DllMain(IntPtr hModule, uint ul_reason_for_call, IntPtr lpReserved)
        {
            switch (ul_reason_for_call)
            {
                case DLL_PROCESS_ATTACH:
                    HModule = hModule;
                    Task.Run(MainThread);
                    Task.Run(DetectExitApplication);
                    break;
                default:
                    break;
            }
            return true;
        }



        private static void MainThread()
        {
            if (NativeAPI.AllocConsole())
            {
                Console.WriteLine($"Hello from native AOT Ptr={HModule.ToString("X")}");
            }
            
        }

        private static async Task DetectExitApplication()
        {
            while (true)
            {
                if (NativeAPI.IsKeyPressed(VirtualKey.VK_ESCAPE))
                {
                    Console.WriteLine($"Detected press key {VirtualKey.VK_ESCAPE.ToString()}");
                    await Task.Delay(2000);
                    break;
                }
                await Task.Delay(10);
            }

            if (!NativeAPI.FreeLibrary(HModule))
            {
                Console.WriteLine("Failed to free library.");
            }
            else
            {
                Console.WriteLine($"Success to free library . {HModule.ToString("X")}");
            }
            await Task.Delay(2000);
            if (!NativeAPI.FreeConsole())
            {
                Console.WriteLine("Failed to free console.");
            }
        }
    }
}
