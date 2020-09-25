namespace ProcessMonitor.UI
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.Windows.Controls;
    using System.Windows.Input;

    /// <summary>
    /// 
    /// </summary>
    public class MainWindowViewModel : BaseViewModel
    {
        public static MainWindowViewModel DesignTimeData => new MainWindowViewModel();


        #region Private fields

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private UserControl _currentMainView;

        #endregion


        #region Public properties

        public UserControl CurrentMainView
        {
            get => _currentMainView;
            set 
            {
                _currentMainView = value;
                OnPropertyChanged();
            }
        }


        #endregion


        #region Commands

        public ICommand BackCommand { get; }

        #endregion


        public MainWindowViewModel()
        {
            BackCommand = new RelayCommand(ExecuteBackCommand);
        }

        private void ExecuteBackCommand()
        {
            var viewChanger = DI.GetService<IMainViewChanger<MainViews>>();

            viewChanger.ChangeView(MainViews.ProcessListView, DI.GetService<ProcessListViewModel>());
        }

    };
};