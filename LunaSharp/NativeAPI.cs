using System.Runtime.InteropServices;

namespace LunaSharp
{
    public static class NativeAPI
    {
        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool AllocConsole();

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool FreeLibrary(IntPtr hModule);

        [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        public static extern int MessageBox(IntPtr hWnd, string text, string caption, uint type);

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool FreeConsole();

        [DllImport("user32.dll")]
        public static extern short GetAsyncKeyState(VirtualKey vKey);

        public static bool IsKeyPressed(VirtualKey key)
        {
            return (GetAsyncKeyState(key) & 0x8000) != 0;
        }


    }

}
