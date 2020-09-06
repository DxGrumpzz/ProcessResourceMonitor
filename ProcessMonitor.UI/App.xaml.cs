namespace ProcessMonitor.UI
{
    using System;
    using System.Runtime.InteropServices;
    using System.Security.RightsManagement;
    using System.Windows;

    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {

        private const string DLL_NAME = "ProcessMonitor.dll";

        private struct ProcessData
        {
            public string ProcessName { get; set; }
            public IntPtr ProcessHandle { get; set; }
            public IntPtr ProcessHWND { get; set; }
        };

        [DllImport(DLL_NAME, CharSet = CharSet.Unicode)]
        private static extern bool GetProcesses(ref IntPtr processDataOut, ref ulong numberOfProcesses);
        [DllImport(DLL_NAME)]
        private static extern bool DeleteUnmangedMemory(ref IntPtr pointr, bool isArray = false);

        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            IntPtr processesPtr = IntPtr.Zero;
            ulong numberOfProcesses = 0;

            GetProcesses(ref processesPtr, ref numberOfProcesses);

            ProcessData[] processes = new ProcessData[numberOfProcesses];

            int jumpOffset = Marshal.SizeOf<ProcessData>();
            IntPtr processesPtrTemp = processesPtr;

            for (ulong a = 0; a < numberOfProcesses; a++)
            {
                processes[a] = Marshal.PtrToStructure<ProcessData>(processesPtrTemp);
                processesPtrTemp = IntPtr.Add(processesPtrTemp, jumpOffset);
            };

            processesPtrTemp = IntPtr.Zero;
            DeleteUnmangedMemory(ref processesPtr, true);


            (Current.MainWindow = new MainWindow())
            .Show();
        }

    };
};
