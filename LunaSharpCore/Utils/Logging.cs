
namespace LunaSharpCore.Utils
{
    public static class Logging
    {
        #region Static Methods

        public static WriteDelegate Write(bool logToFile = false, string memberName = "") =>
            (value, args) =>
            {
                object finalMessage = value;
                if (args.Length > 0)
                    try
                    {
                        finalMessage = string.Format(value.ToString(), args);
                    }
                    catch (Exception)
                    {
                        // Ignored.
                    }

                Write(finalMessage.ToString(), logToFile, memberName);
            };

        public static void Write(object value, object[] args, bool logToFile = false, string memberName = "")
        {
            object finalMessage = value;
            if (args.Length > 0)
                try
                {
                    finalMessage = string.Format(value.ToString(), args);
                }
                catch (Exception)
                {
                    // Ignored.
                }

            Write(finalMessage.ToString(), logToFile, memberName);
        }

        public static void LogAllExceptions()
        {
            AppDomain.CurrentDomain.UnhandledException += (sender, args) =>
            {
                if (args.ExceptionObject is Exception exception)
                    Write(true)(exception.Message);
            };
        }

        private static void Write(string message, bool logToFile, string memberName)
        {
            string format = $"[{DateTime.Now.TimeOfDay}]: ({memberName}) -> {message}";
            Console.WriteLine(message);
            if (!logToFile) return;

            try
            {
                using (StreamWriter writer = new StreamWriter(Path.Combine(ApplicationInfo.DomainBaseDir, ApplicationInfo.LogFileName), true))
                {
                    writer.WriteLine(format);
                }
            }
            catch (Exception)
            {
                // Ignored.
            }
        }

        #endregion

        #region Nested Types, Enums, Delegates

        public delegate void WriteDelegate(object value, params object[] args);

        #endregion
    }
}
