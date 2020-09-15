namespace ProcessMonitor.UI
{
    using System.Windows.Controls;

    /// <summary>
    /// Interaction logic for ProcessListView.xaml
    /// </summary>
    public partial class ProcessListView : UserControl
    {

        public ProcessListView()
        {
            InitializeComponent();
        }

        public ProcessListView(ProcessListViewModel viewModel)
        {
            InitializeComponent();
            DataContext = viewModel;
        }

    };
};
