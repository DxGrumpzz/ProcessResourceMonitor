namespace ProcessMonitor.UI
{
    using System.Collections.Generic;

    /// <summary>
    /// 
    /// </summary>
    public class ProcessListViewModel
    {
        public static ProcessListViewModel DesignTimeData =>
           new ProcessListViewModel(new List<ProcessDataListItemViewModel>()
           {
                new ProcessDataListItemViewModel(new ProcessData()),
                new ProcessDataListItemViewModel(new ProcessData()),
                new ProcessDataListItemViewModel(new ProcessData()),
                new ProcessDataListItemViewModel(new ProcessData()),
           });


        #region Private fields

        #endregion


        #region Public properties


        public IList<ProcessDataListItemViewModel> ProcessList { get; set; }


        #endregion


        #region Commands

        #endregion


        public ProcessListViewModel(IList<ProcessDataListItemViewModel> processes)
        {
            ProcessList = processes;
        }


    };
};