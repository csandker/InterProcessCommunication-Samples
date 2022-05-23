using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NtApiDotNet;
using NtApiDotNet.Ndr;
using NtApiDotNet.Win32;
//using NtObjectManager;

namespace ProcessExports
{
    class Program
    {
        static void Main(string[] args)
        {
            int iProcesses = 0;
            int iConnectPorts = 0;
            int iCommPorts = 0;
            
            IEnumerable<NtProcess> processes = NtProcess.GetProcesses(ProcessAccessRights.VmRead | ProcessAccessRights.QueryInformation);
            foreach (var process in processes){
                // maybe later...
                //ISymbolResolver resolver = new SymbolResolverWrapper(process.Is64Bit, SymbolResolver.Create(process, "C:\\Users\\LocalAdm\\Desktop\\dbghelp.dll", "C:\\Windows\\Symbols"));
                //IntPtr address = resolver.GetAddressOfSymbol("ntdll!NtRenameKey
                // maybe later...
                //IPeb peb = process.GetPeb();
                //IntPtr baseAddr = process.ImageBaseAddress;
                IEnumerable<NtHandle> handles = Enumerable.Empty<NtHandle>(); ;
                try {
                    handles = process.GetHandles(true);
                }
                catch (Exception ex) {
                    // failed to get handles, could be many reasons, we don't care and continue
                    continue;
                }
                iProcesses += 1;
                Console.Write("[*] Process: {0} [PID: {1}]\n", process.Name, process.ProcessId);
                foreach (var handle in handles)
                {
                    if (handle.ObjectType.Equals("ALPC Port"))
                    {
                        var portFlags = "";
                        try
                        {
                            NtAlpc alpcObject = (NtAlpc)handle.GetObject();
                            portFlags = alpcObject.Flags.ToString();
                        }
                        catch { }
                        if (handle.Name.Length > 0)
                        {
                            iConnectPorts += 1;
                            Console.Write("  [+] Found handle to ALPC Connection Port: {0}\n", handle.Name);
                            if (handle.SecurityDescriptor != null)
                            {
                                Console.Write("    Object Owner: {0}\n", handle.SecurityDescriptor.Owner);
                                Console.Write("    DACL: {0}\n", handle.SecurityDescriptor);
                                if (portFlags.Length > 0) Console.Write("    Flags: {0}\n", portFlags);
                            }
                        }else { 
                            iCommPorts += 1;
                            Console.Write("  [+] Found handle to ALPC Communication Port\n");
                            if(portFlags.Length > 0) Console.Write("    Flags: {0}\n", portFlags);
                        }
                    }
                }
            }
            Console.Write("\n\n[*] Stats:\n Processes: {0}\n ALPC Connection Ports: {1}\n ALPC Communication Ports: {2}\n Avg. connection ports per: {3:0.0#}\n Avg. communication port per process: {4:0.0#}",
                iProcesses, iConnectPorts, iCommPorts, ((float)iConnectPorts/iProcesses), ((float)iCommPorts/iProcesses) );
        }
    }

    // maybe later ...
    /*internal class SymbolResolverWrapper : ISymbolResolver
    {
        private readonly ISymbolResolver _resolver;
        private readonly SymbolLoadedModule _base_module;
        private readonly Dictionary<string, int> _resolved;

        static readonly Dictionary<string, int> _resolved_32bit = new Dictionary<string, int>();
        static readonly Dictionary<string, int> _resolved_64bit = new Dictionary<string, int>();
        static readonly string _dllname = "ntdll";
        static readonly string _dllprefix = $"{_dllname}!";

        public SymbolResolverWrapper(bool is64bit, ISymbolResolver resolver)
        {
            _resolver = resolver;
            foreach (var module in _resolver.GetLoadedModules())
            {
                Console.Write("Module: {0}\n", module.Name);
                if (module.Name.Equals(_dllname, StringComparison.OrdinalIgnoreCase))
                {
                    _base_module = module;
                    Console.WriteLine("Found NTDLL module\n");
                    break;
                }
            }

            _resolved = is64bit ? _resolved_64bit : _resolved_32bit;
        }

        internal static Dictionary<string, int> GetResolved32Bit()
        {
            return _resolved_32bit;
        }

        internal static Dictionary<string, int> GetResolvedNative()
        {
            if (Environment.Is64BitProcess)
            {
                return _resolved_64bit;
            }
            else
            {
                return _resolved_32bit;
            }
        }

        public IEnumerable<SymbolLoadedModule> GetLoadedModules()
        {
            return _resolver.GetLoadedModules();
        }

        public IEnumerable<SymbolLoadedModule> GetLoadedModules(bool refresh)
        {
            return _resolver.GetLoadedModules(refresh);
        }

        public SymbolLoadedModule GetModuleForAddress(IntPtr address)
        {
            return _resolver.GetModuleForAddress(address);
        }

        public SymbolLoadedModule GetModuleForAddress(IntPtr address, bool refresh)
        {
            return _resolver.GetModuleForAddress(address, refresh);
        }

        public string GetModuleRelativeAddress(IntPtr address)
        {
            return _resolver.GetModuleRelativeAddress(address);
        }

        public string GetModuleRelativeAddress(IntPtr address, bool refresh)
        {
            return _resolver.GetModuleRelativeAddress(address, refresh);
        }

        public IntPtr GetAddressOfSymbol(string symbol)
        {
            if (_resolved.ContainsKey(symbol) && _base_module != null && symbol.StartsWith(_dllprefix))
            {
                return _base_module.BaseAddress + _resolved[symbol];
            }

            IntPtr ret = _resolver.GetAddressOfSymbol(symbol);
            if (ret != IntPtr.Zero && symbol.StartsWith(_dllprefix))
            {
                _resolved[symbol] = (int)(ret.ToInt64() - _base_module.BaseAddress.ToInt64());
            }

            return ret;
        }

        public string GetSymbolForAddress(IntPtr address)
        {
            return _resolver.GetSymbolForAddress(address);
        }

        public string GetSymbolForAddress(IntPtr address, bool generate_fake_symbol)
        {
            return _resolver.GetSymbolForAddress(address, generate_fake_symbol);
        }

        public string GetSymbolForAddress(IntPtr address, bool generate_fake_symbol, bool return_name_only)
        {
            return _resolver.GetSymbolForAddress(address, generate_fake_symbol, return_name_only);
        }

        public void Dispose()
        {
            _resolver.Dispose();
        }

        public void ReloadModuleList()
        {
            _resolver.ReloadModuleList();
        }

        public void LoadModule(string module_path, IntPtr base_address)
        {
            _resolver.LoadModule(module_path, base_address);
        }
    }*/
}
