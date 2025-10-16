using System.Collections.Generic;
using System.Linq;

namespace JX1Launcher.Models
{
    /// <summary>
    /// Server cluster information
    /// </summary>
    public class ServerCluster
    {
        public string Name { get; set; } = "";
        public List<ServerInfo> Servers { get; set; } = new List<ServerInfo>();
    }

    /// <summary>
    /// Individual server information
    /// </summary>
    public class ServerInfo
    {
        public string Name { get; set; } = "";
        public string IpAddress { get; set; } = "";
        public int Port { get; set; } = 0;
        public bool IsOnline { get; set; } = true;
    }

    /// <summary>
    /// Server configuration manager
    /// Provides predefined server clusters and servers
    /// </summary>
    public static class ServerConfig
    {
        private static List<ServerCluster> _clusters;

        static ServerConfig()
        {
            // Initialize with default clusters
            _clusters = new List<ServerCluster>
            {
                new ServerCluster
                {
                    Name = "Cụm 1 - Trung Nguyên",
                    Servers = new List<ServerInfo>
                    {
                        new ServerInfo { Name = "Tân Thủ 1", IpAddress = "103.x.x.1", Port = 5816 },
                        new ServerInfo { Name = "Tân Thủ 2", IpAddress = "103.x.x.2", Port = 5816 },
                        new ServerInfo { Name = "Tân Thủ 3", IpAddress = "103.x.x.3", Port = 5816 },
                        new ServerInfo { Name = "Võ Lâm 1", IpAddress = "103.x.x.4", Port = 5816 },
                        new ServerInfo { Name = "Võ Lâm 2", IpAddress = "103.x.x.5", Port = 5816 }
                    }
                },
                new ServerCluster
                {
                    Name = "Cụm 2 - Hoa Sơn",
                    Servers = new List<ServerInfo>
                    {
                        new ServerInfo { Name = "Hoa Sơn 1", IpAddress = "103.x.x.10", Port = 5816 },
                        new ServerInfo { Name = "Hoa Sơn 2", IpAddress = "103.x.x.11", Port = 5816 },
                        new ServerInfo { Name = "Hoa Sơn 3", IpAddress = "103.x.x.12", Port = 5816 }
                    }
                },
                new ServerCluster
                {
                    Name = "Cụm 3 - Thiên Sơn",
                    Servers = new List<ServerInfo>
                    {
                        new ServerInfo { Name = "Thiên Sơn 1", IpAddress = "103.x.x.20", Port = 5816 },
                        new ServerInfo { Name = "Thiên Sơn 2", IpAddress = "103.x.x.21", Port = 5816 },
                        new ServerInfo { Name = "Thiên Sơn 3", IpAddress = "103.x.x.22", Port = 5816 }
                    }
                }
            };
        }

        /// <summary>
        /// Get all server clusters
        /// </summary>
        public static List<ServerCluster> GetClusters()
        {
            return new List<ServerCluster>(_clusters);
        }

        /// <summary>
        /// Get cluster by name
        /// </summary>
        public static ServerCluster? GetCluster(string name)
        {
            return _clusters.FirstOrDefault(c => c.Name == name);
        }

        /// <summary>
        /// Get server by cluster and server name
        /// </summary>
        public static ServerInfo? GetServer(string clusterName, string serverName)
        {
            var cluster = GetCluster(clusterName);
            return cluster?.Servers.FirstOrDefault(s => s.Name == serverName);
        }

        /// <summary>
        /// Add custom cluster
        /// </summary>
        public static void AddCluster(ServerCluster cluster)
        {
            _clusters.Add(cluster);
        }

        /// <summary>
        /// Add server to existing cluster
        /// </summary>
        public static void AddServer(string clusterName, ServerInfo server)
        {
            var cluster = GetCluster(clusterName);
            if (cluster != null)
            {
                cluster.Servers.Add(server);
            }
        }
    }
}
