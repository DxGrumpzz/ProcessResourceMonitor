namespace ProcessMonitor.UI
{
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.Windows.Controls;

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

        #endregion


        public MainWindowViewModel()
        {

        }



    };
};