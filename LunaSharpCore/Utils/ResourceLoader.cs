namespace LunaSharpCore.Utils
{
    public static class ResourceLoader
    {
        public static IntPtr LoadDLL(string dllName)
        {
            IntPtr handle = WinAPI.GetModuleHandle(dllName);
            if (handle != IntPtr.Zero)
            {
                Logging.Write(true)($"{dllName} is loaded at {handle:X}");
                return handle;
            }
            var pathToFile = Path.Combine(ApplicationInfo.DomainBaseDir, dllName);
            if (!File.Exists(pathToFile))
            {
                Logging.Write(true)($"Could not found dll path {pathToFile}");
                return IntPtr.Zero;
            }
            var ret = WinAPI.LoadLibrary(pathToFile);
            handle = WinAPI.GetModuleHandle(dllName);
            Logging.Write(true)($"{pathToFile} is loaded at {handle:X}");
            return ret;
        }
    }
}
