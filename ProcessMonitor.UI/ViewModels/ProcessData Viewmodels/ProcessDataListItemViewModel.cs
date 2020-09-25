namespace ProcessMonitor.UI
{
    using System;
    using System.Windows.Input;

    /// <summary>
    /// 
    /// </summary>
    public class ProcessDataListItemViewModel
    {

        #region Private fields

        #endregion


        #region Public properties

        public ProcessData Process { get; }

        #endregion


        #region Commands

        public ICommand ProcessClickCommand { get; set; }

        #endregion


        public ProcessDataListItemViewModel(ProcessData process)
        {
            Process = process;

            ProcessClickCommand = new RelayCommand(ExecuteProcessClickCommand);
        }


        private void ExecuteProcessClickCommand()
        {
            var viewChanger = DI.GetService<IMainViewChanger<MainViews>>();

            viewChanger.ChangeView(MainViews.ProcessItemView, new ProcessItemViewModel(Process));
        }

    };
};