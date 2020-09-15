namespace ProcessMonitor.UI
{
    using System;
    using System.Collections.Generic;


    /// <summary>
    /// An enumerator for the type of services available
    /// </summary>
    public enum ServiceType
    { 
        /// <summary>
        /// A single unique instance of a service that will retireved every time <see cref="DI.GetService{T}"/> is called
        /// </summary>
        Singleton = 0,

        /// <summary>
        /// A service that will be instanciated with every call to <see cref="DI.GetService{T}"/> 
        /// </summary>
        Transient = 1,
    };

    
    /// <summary>
    /// A DI container for the application
    /// </summary>
    public class DI
    {
        /// <summary>
        /// A dictionary that will contain a list of services
        /// </summary>
        private Dictionary<string, Tuple<object, ServiceType>> _services;

        public DI()
        {
            _services = new Dictionary<string, Tuple<object, ServiceType>>();
        }


        /// <summary>
        /// Add a service as a singelton
        /// </summary>
        /// <typeparam name="T"> The service type </typeparam>
        /// <param name="service"> The service instance that will be retrieved </param>
        public void AddSingelton<T>(T service)
        {
            // Some unique service identifier
            string serviceID = GetServiceID<T>();

            // Add the service as a singleton to the services list
            _services.Add(serviceID, new Tuple<object, ServiceType>(service, ServiceType.Singleton));
        }


        /// <summary>
        /// Add a default instance of a singelton service
        /// </summary>
        /// <typeparam name="T"> The service type </typeparam>
        public void AddSingelton<T>() 
            where T : new()
        {
            // Add the singelton service and call the default constructor
            AddSingelton<T>(new T());
        }


        /// <summary>
        /// Add a service as s transient instnace that will be "newed" on every call
        /// </summary>
        /// <typeparam name="T"> The service type </typeparam>
        /// <param name="serviceFactory"> A 'factory' function that will be called to create the new instance </param>
        public void AddTransient<T>(Func<T> serviceFactory)
        {
            // Create a unique service identifier
            string serviceID = GetServiceID<T>();

            // Add the service to the services list as a transient
            _services.Add(serviceID, new Tuple<object, ServiceType>(serviceFactory, ServiceType.Transient));
        }


        /// <summary>
        /// Retrieves a service based on it's type
        /// </summary>
        /// <typeparam name="T"> The service type to retrieve </typeparam>
        /// <returns></returns>
        public T GetService<T>()
        {
            // Find service ID
            string serviceType = GetServiceID<T>();

            // Find the service 
            bool result = _services.TryGetValue(serviceType, out Tuple<object, ServiceType> service);

            // If service is not found
            if (result == false)
                // Notifiy caller
                throw new Exception($"Service {typeof(T)} does not exist");

            // If the service was bound as a transient 
            if(service.Item2 == ServiceType.Transient)
            {
                // Get the service factory 
                Func<T> serviceFactory = (Func<T>)service.Item1;

                // Call the factory function and return an instnace
                return serviceFactory();
            };

            
            return (T)service.Item1;
        }


        /// <summary>
        /// Retrieves a *Hopefully* unique identifier for a service
        /// </summary>
        /// <typeparam name="T"> The service type </typeparam>
        /// <returns> Returns a string based on the Assembly Qualified Type name </returns>
        private string GetServiceID<T>()
        {
            string typeID = typeof(T).AssemblyQualifiedName;

            return typeID;
        }

    };
};