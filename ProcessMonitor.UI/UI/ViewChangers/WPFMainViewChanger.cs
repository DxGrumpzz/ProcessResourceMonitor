namespace ProcessMonitor.UI
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
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
        private Dictionary<TViewKey, Type> _views;

        /// <summary>
        /// The MainViewmodel model. This is here because the "main view" is stored there
        /// </summary>
        private MainWindowViewModel _mainWindowViewModel;


        public WPFMainViewChanger(MainWindowViewModel mainWindowViewModel, Dictionary<TViewKey, Type> views)
        {
            _mainWindowViewModel = mainWindowViewModel;
            _views = views;
        }


        public void ChangeView(TViewKey viewKey)
        {
            // Find the view 
            var viewType = _views[viewKey];

            // Get default constructor
            var viewCtor = viewType.GetConstructor(Type.EmptyTypes);

            // Create new view
            var view = (UserControl)viewCtor.Invoke(null);

            // Update main view
            _mainWindowViewModel.CurrentMainView = view;
        }


        public void ChangeView<T>(TViewKey viewKey, T viewmodel)
        {
            // Find the view 
            var viewType = _views[viewKey];

            // Find matching constructor
            var viewCtor = viewType.GetConstructor(new[] { typeof(T) });

            // Create new view object and pass it the argument
            var view = (UserControl)viewCtor.Invoke(new[] { (object)viewmodel });

            // Update the view
            _mainWindowViewModel.CurrentMainView = view;
        }
        

        public void ChangeView(TViewKey viewKey, params object[] args)
        {
            // Find the view 
            var viewType = _views[viewKey];

            // Convert args array to Type array
            var constructorArgsTypeArray = args
                .Select(arg => arg.GetType())
                .ToArray();

            // Find matching constructor
            var viewCtor = viewType.GetConstructor(constructorArgsTypeArray);

            // Invoke view constructor and pass the arguments
            var view = (UserControl)viewCtor.Invoke(args);

            // Update main view
            _mainWindowViewModel.CurrentMainView = view;
        }

    }
};