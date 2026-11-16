using System;
using System.Diagnostics;
using System.IO;
using System.Threading.Tasks;
namespace Interfaz.Models;

public class Miner{
    //para ejecutar el minero en c++
    private Process proceso;
    private string ruta;
    private string directorio;
    //usa la ruta del minero en /src/Minero    
    public Miner(string directorio){
	// ruta = "../build/Minero";
	string s = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),"rolas");
	ruta = Path.Combine(s, "Minero");
	 if(!File.Exists(ruta)){
	     throw new FileNotFoundException("No se encontró el ejecutable Minero en :",ruta);
            }
	// ruta = Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "../../../../../build/Minero"));
	proceso = new Process();
	this.directorio = directorio;
	//para redirigir la entrada estandar y asignar el process	
	proceso.StartInfo.FileName = ruta;
        proceso.StartInfo.UseShellExecute = false;
        proceso.StartInfo.RedirectStandardInput = true;
	//solo si quiero poner una ventanita para ver como va el minero, pero porque haría eso?
        // proceso.StartInfo.RedirectStandardOutput = true;
        // proceso.StartInfo.RedirectStandardError = true;
        proceso.StartInfo.CreateNoWindow = true;
    }    
    public async Task ejecuta(){
	proceso.Start();
	proceso.StandardInput.WriteLine(directorio);
        proceso.StandardInput.Close();
	await proceso.WaitForExitAsync();	
    }    
}
