namespace ProcessMonitor.UI
{
    using System.Windows;
    using System.Windows.Controls;

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        ProcessData[] _processList;

        public MainWindow()
        {
            InitializeComponent();
        }

        public MainWindow(ProcessData[] processData)
        {
            _processList = processData;
            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            foreach (var process in _processList)
            {
                MainStackPanel.Children.Add(new TextBlock()
                {
                    Text = process.ProcessName,
                });
            };
        }
    };
};