namespace ProcessMonitor.UI
{
    using System.Windows.Controls;

    /// <summary>
    /// Interaction logic for ProcessItemView.xaml
    /// </summary>
    public partial class ProcessItemView : UserControl
    {

        public ProcessItemView(ProcessItemViewModel viewModel)
        {
            InitializeComponent();
            DataContext = viewModel;
        }

        public ProcessItemView()
        {
            InitializeComponent();
        }
    }
}
