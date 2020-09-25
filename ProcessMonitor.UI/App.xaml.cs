namespace ProcessMonitor.UI
{
    using System;
    using System.Linq;
    using System.Runtime.InteropServices;
    using System.Windows;

    public class ProcessData
    {
        public string ProcessName { get; set; }
        public IntPtr ProcessHandle { get; set; }
        public ulong ProcessID { get; set; }
        public IntPtr ProcessHWND { get; set; }
    };

    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {

        private const string DLL_NAME = "ProcessMonitor.dll";

        private struct ProcessDataStruct
        {
            public IntPtr ProcessNamePtr;
            public IntPtr ProcessHandle;
            public ulong ProcessID;
            public IntPtr ProcessHWND;

            public ProcessDataStruct(IntPtr processNamePtr, IntPtr processHandle, ulong processID, IntPtr processHWND)
            {
                ProcessNamePtr = processNamePtr;
                ProcessHandle = processHandle;
                ProcessID = processID;
                ProcessHWND = processHWND;
            }
        };



        [DllImport(DLL_NAME, CharSet = CharSet.Unicode)]
        private static extern bool GetProcesses(ref IntPtr processDataOut, ref ulong numberOfProcesses);
        [DllImport(DLL_NAME)]
        private static extern bool DeleteUnmangedMemory(ref IntPtr pointr, bool isArray = false);



        private ProcessData[] GetProcesses()
        {
            IntPtr processesPtr = IntPtr.Zero;
            ulong numberOfProcesses = 0;

            GetProcesses(ref processesPtr, ref numberOfProcesses);

            ProcessData[] processes = new ProcessData[numberOfProcesses];

            int jumpOffset = Marshal.SizeOf<ProcessDataStruct>();
            IntPtr processesPtrTemp = processesPtr;

            for (ulong a = 0; a < numberOfProcesses; a++)
            {
                ProcessDataStruct processDataStruct = Marshal.PtrToStructure<ProcessDataStruct>(processesPtrTemp);

                processes[a] = new ProcessData()
                {
                    ProcessHandle = processDataStruct.ProcessHandle,
                    ProcessHWND = processDataStruct.ProcessHWND,
                    ProcessID = processDataStruct.ProcessID,
                    ProcessName = Marshal.PtrToStringUni(processDataStruct.ProcessNamePtr),
                };

                DeleteUnmangedMemory(ref processDataStruct.ProcessNamePtr, true);

                processesPtrTemp = IntPtr.Add(processesPtrTemp, jumpOffset);
            };

            DeleteUnmangedMemory(ref processesPtr, true);

            return processes;
        }

        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);


            DI di = CreateDI();

            (Current.MainWindow = new MainWindow(di.GetService<MainWindowViewModel>()))
            .Show();
        }


        private DIContainer CreateDIContainer()
        {
            DIContainer diContainer = new DIContainer();

            // Bind ProcessListViewModel
            diContainer.AddSingelton<ProcessListViewModel>(
                new ProcessListViewModel(GetProcesses()
                .Select(process => new ProcessDataListItemViewModel(process))
                .ToList()));

            // Bind MainWindow viewmodel
            diContainer.AddSingelton<MainWindowViewModel>(
                new MainWindowViewModel()
                {
                    CurrentMainView = new ProcessListView(diContainer.GetService<ProcessListViewModel>())
                });

            return di;
        }

    };
};