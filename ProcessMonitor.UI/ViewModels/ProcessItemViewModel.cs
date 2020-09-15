namespace ProcessMonitor.UI
{
    using System;
    using System.Windows.Input;

    /// <summary>
    /// 
    /// </summary>
    public class ProcessItemViewModel
    {

        #region Design data

        public static ProcessItemViewModel DesignTimeData =>
            new ProcessItemViewModel(new ProcessData());


        #endregion

        #region Private fields

        #endregion


        #region Public properties

        public ProcessData ProcessData { get; }

        #endregion


        #region Commands

        #endregion


        public ProcessItemViewModel(ProcessData processData)
        {
            ProcessData = processData;

        }



    };
};