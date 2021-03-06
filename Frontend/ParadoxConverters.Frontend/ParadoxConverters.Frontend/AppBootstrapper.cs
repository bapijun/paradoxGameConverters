

namespace Frontend.Client
{
    using Caliburn.Micro;
    using Frontend.Core.Logging;
    using Frontend.Core.ViewModels;
    using Frontend.Core.ViewModels.Interfaces;
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Reflection;
    using System.Windows;

	public class AppBootstrapper : BootstrapperBase
	{
		SimpleContainer container;
        private IEventAggregator eventAggregator;

		public AppBootstrapper()
		{
			Start();
		}

        protected IEventAggregator EventAggregator
        {
            get
            {
                return this.eventAggregator ?? (this.eventAggregator = this.container.GetInstance<IEventAggregator>());
            }
        }

		protected override void Configure()
		{
			container = new SimpleContainer();

			container.Singleton<IWindowManager, WindowManager>();
			container.Singleton<IEventAggregator, EventAggregator>();
			container.PerRequest<IShell, ShellViewModel>();
            container.PerRequest<IFrameViewModel, FrameViewModel>();
            container.PerRequest<IWelcomeViewModel, WelcomeViewModel>();
		}

		protected override object GetInstance(Type service, string key)
		{
			var instance = container.GetInstance(service, key);
			if (instance != null)
				return instance;

			throw new InvalidOperationException("Could not locate any instances.");
		}

        protected override IEnumerable<Assembly> SelectAssemblies()
        {
            var assemblies = base.SelectAssemblies().ToList();

            // This includes the Frontend.Core assembly.
            assemblies.Add(Assembly.GetAssembly(typeof(FrameViewModel)));

            return assemblies;
        }

		protected override IEnumerable<object> GetAllInstances(Type service)
		{
			return container.GetAllInstances(service);
		}

		protected override void BuildUp(object instance)
		{
			container.BuildUp(instance);
		}

		protected override void OnStartup(object sender, StartupEventArgs e)
		{
			DisplayRootViewFor<IShell>();

            var logEntry = new LogEntry("Startup complete: Application was started from", LogEntrySeverity.Info, LogEntrySource.UI, Environment.CurrentDirectory);

            this.EventAggregator.PublishOnUIThread(logEntry);
		}

        protected override void OnUnhandledException(object sender, System.Windows.Threading.DispatcherUnhandledExceptionEventArgs e)
        {
            this.EventAggregator.PublishOnUIThread(new LogEntry("Error occured: " + e.Exception.Message, LogEntrySeverity.Error, LogEntrySource.UI));
        }
	}
}