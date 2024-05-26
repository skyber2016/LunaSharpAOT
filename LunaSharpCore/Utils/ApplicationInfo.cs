namespace LunaSharpCore.Utils
{
    public static class ApplicationInfo
    {
        #region Fields


        /// <summary>
        ///     Injected Application Path
        /// </summary>
        public static readonly string DomainBaseDir = AppDomain.CurrentDomain.BaseDirectory;
        public static readonly string AppDir = Directory.GetCurrentDirectory();

        /// <summary>
        ///     Log File Name
        /// </summary>
        public static readonly string LogFileName = DateTime.Now.ToString("d").Replace('/', '-') + ".log";

        #endregion
    }
}
