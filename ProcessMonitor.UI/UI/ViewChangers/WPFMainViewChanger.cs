namespace ProcessMonitor.UI
{
    using System.Collections.Generic;
    using System.Windows.Controls;
    
    /// <summary>
    /// An enumaration of possible main views
    /// </summary>
    public enum MainViews
    {
        ProcessListView = 0,
        ProcessItemView = 1,
        View2 = 2,
        View3 = 3,
    }


    /// <summary>
    /// An implementation of <see cref="IMainViewChanger{TViewKey}"/> for WPF
    /// </summary>
    /// <typeparam name="TViewKey"></typeparam>
    public class WPFMainViewChanger<TViewKey> : IMainViewChanger<TViewKey>
    {
        /// <summary>
        /// A dictionary containing view keys and views
        /// </summary>
        private Dictionary<TViewKey, UserControl> _views;

        /// <summary>
        /// The MainViewmodel model. This is here because the "main view" is stored there
        /// </summary>
        private MainWindowViewModel _mainWindowViewModel;

        public WPFMainViewChanger(MainWindowViewModel mainWindowViewModel, Dictionary<TViewKey, UserControl> views)
        {
            _mainWindowViewModel = mainWindowViewModel;
            _views = views;
        }

        public void ChangeView(TViewKey viewKey)
        {
            // Find the view 
            var view = _views[viewKey];

            // Change the view
            _mainWindowViewModel.CurrentMainView = view;
        }

        public void ChangeView<T>(TViewKey viewKey, T viewmodel)
        {
            // Find the view 
            var view = _views[viewKey];

            // For now only change the data context of the view to the given T viewmodel
            view.DataContext = viewmodel;

            // Update the view
            _mainWindowViewModel.CurrentMainView = view;
        }

    }
};