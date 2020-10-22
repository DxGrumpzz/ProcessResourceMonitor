namespace ProcessMonitor.UI
{
    /// <summary>
    /// An interface for a view changer
    /// </summary>
    /// <typeparam name="TViewKey"></typeparam>
    public interface IMainViewChanger<TViewKey>
    {
        /// <summary>
        /// Change the main application view
        /// </summary>
        /// <param name="viewKey"> A view key that will be used to find the correct view </param>
        public void ChangeView(TViewKey viewKey);

        /// <summary>
        /// Change the main application view
        /// </summary>
        /// <typeparam name="T"> The viewmodel type </typeparam>
        /// <param name="viewKey"> A view key that will be used to find the correct view </param>
        /// <param name="viewArguments"> arguments that can be passed to the view </param>
        public void ChangeView<T>(TViewKey viewKey, T viewArguments);

        /// <summary>
        /// Changes the main application view, with any given argument
        /// </summary>
        /// <param name="viewKey"> A view key that will be used to find the correct view </param>
        /// <param name="args"> Any ol' arguments that a view needs </param>
        public void ChangeView(TViewKey viewKey, params object[] args);

    };
};