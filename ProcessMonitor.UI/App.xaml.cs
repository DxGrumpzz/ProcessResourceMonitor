﻿namespace ProcessMonitor.UI
{
    using System.Windows;

    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {

        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            (Current.MainWindow = new MainWindow())
            .Show();
        }

    };
};
