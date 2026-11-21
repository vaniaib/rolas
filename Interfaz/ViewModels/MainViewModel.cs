using Avalonia.Platform.Storage;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using System.Threading;
using System.Threading.Tasks;
using System;
using System.Collections.ObjectModel;
using System.Linq;
namespace Interfaz.ViewModels;
using Interfaz.Models;

public partial class MainViewModel : ObservableObject
{    
    private readonly IStorageProvider _storageProvider;
    private readonly aplicacionMiner _aplicacionMiner;
    private readonly Bdd _baseDatos;  
    private CancellationTokenSource? _busquedaCts;
    public ObservableCollection<Rola> Rolas { get; } = new();
    public ObservableCollection<Elemento> Elementos { get; } = new();
    //public partial string Greeting { get; set; } = "Welcome to Avalonia!";
    [ObservableProperty]
    private string rutaaMinar = "";
    [ObservableProperty]
    private bool procesando = false;
    [ObservableProperty]
    private string busqueda = "";
    [ObservableProperty]
    private bool mostrandoRolas = true;
    [ObservableProperty]
    private bool mostrandoLista = false;
    [ObservableProperty]
    private Rola? rolaSeleccionada;
    [ObservableProperty]
    private Elemento? elementoSeleccionado;
    [ObservableProperty]
    private string tablaActual = "rolas";
    
    public MainViewModel(IStorageProvider storageProvider){
        _storageProvider = storageProvider;
	_aplicacionMiner = new aplicacionMiner();
	_baseDatos = new Bdd();
    }

//para minar, pero le dejé el nombre seleccionaruta pq me confunfo y así :(
[RelayCommand]
private async Task seleccionaRuta(){
    var directorios = await _storageProvider.OpenFolderPickerAsync(
    new FolderPickerOpenOptions{
		Title = "Selecciona un directorio",
		AllowMultiple = false
    });
	if (directorios.Count > 0){
	    RutaaMinar = directorios[0].TryGetLocalPath() ?? "";
	    if(!string.IsNullOrEmpty(RutaaMinar)){
		Procesando = true;
		try{
                    await _aplicacionMiner.mina(RutaaMinar);
                }
                finally{
                    Procesando = false;
                }		
	    }       
	}
    }
    //enseñar rolas en la columana de enmedio
    [RelayCommand]
    private async Task mostrarRolas(){
        // TablaActual = "rolas";
        // MostrandoRolas = true;
        // MostrandoLista = false;
        // await cargarRolas();
	await mostrarTabla("rolas");
    }
    private async Task cargarRolas(){
        var canciones = await _baseDatos.getRolas();
        Rolas.Clear();
        foreach (var cancion in canciones){
            Rolas.Add(cancion);
        }
    }

//la canción seleccionada

[RelayCommand]
private void seleccionarRola(Rola rola){
        RolaSeleccionada = rola;
        ElementoSeleccionado = null;
}
// segun la tabla que pasen es la que muestra
[RelayCommand]
    private async Task mostrarTabla(string tabla) {
        TablaActual = tabla;
	Busqueda = "";
        RolaSeleccionada = null;
        ElementoSeleccionado = null;
        MostrandoRolas = (tabla == "rolas");
	MostrandoLista = (tabla != "rolas");
        if (tabla == "rolas"){
            await cargarRolas();
            return;
        }
        Elementos.Clear();
        var registros = await _baseDatos.getElementos(tabla);
        foreach (var elemento in registros){
            Elementos.Add(elemento);
        }
    }
    // algo que no es rola
    [RelayCommand]
    private void seleccionarElemento(Elemento elemento){
        ElementoSeleccionado = elemento;
	RolaSeleccionada = null;
    }
    // para essoger la portada amanito
    [RelayCommand]
    private async Task seleccionarImagen(){
        if (RolaSeleccionada == null)
            return;
        var archivos = await _storageProvider.OpenFilePickerAsync(
            new FilePickerOpenOptions{
                Title = "Seleccionar portada",
                AllowMultiple = false,
                FileTypeFilter =[
                    new FilePickerFileType("Imágenes"){
                        Patterns = ["*.png", "*.jpg", "*.jpeg", "*.webp"]
			    }
                ]
            });
        if (archivos.Count == 0)
            return;
	string? ruta = archivos[0].TryGetLocalPath();
	// if (!string.IsNullOrEmpty(ruta)){
        //     RolaSeleccionada.CoverPath = ruta;
        // }
	if (string.IsNullOrEmpty(ruta)){
	    Console.WriteLine("No se pudo obtener la ruta de la imagen");
	    return;
    }
    RolaSeleccionada.CoverPath = ruta;
    Console.WriteLine($"Portada seleccionada: {ruta}");
    // RolaSeleccionada.CoverPath = archivos[0].TryGetLocalPath() ?? "";
    }
    // guardar camibos
    [RelayCommand]
    private async Task guardarRola(){
        if (RolaSeleccionada == null)
            return;
	int id = RolaSeleccionada.IdRola;
        await _baseDatos.editaRola(RolaSeleccionada);
        await cargarRolas();
        // Volver a seleccionar la canción actualizada
        RolaSeleccionada = Rolas.FirstOrDefault(r => r.IdRola == id);
    }
    // buscar
    partial void OnBusquedaChanged(string value){
        if (TablaActual == "rolas"){
	    // _ = filtrarRolas();
	    _ = filtrarRolas(value);	    
            // _ = buscar();
        }
    }

private async Task buscar(){
        _busquedaCts?.Cancel();
        var cts = new CancellationTokenSource();
        _busquedaCts = cts;
        try{
            await Task.Delay(300, cts.Token);
            if (!cts.IsCancellationRequested)
                await cargarRolas();
        }
        catch (OperationCanceledException){
        }
    }
    private async Task filtrarRolas(string busqueda){
    try{
        var canciones = await _baseDatos.getRolas(busqueda);
        Rolas.Clear();
        foreach (var cancion in canciones){
            Rolas.Add(cancion);
        }
    }catch (Exception ex){
        Console.WriteLine("Error al buscar: " + ex.Message);
    }
}
    // private async Task filtrarRolas(){
    //     var canciones = await _baseDatos.getRolas();
    //     var filtradas = canciones.Where(r =>
    //         r.Title.Contains(Busqueda, StringComparison.OrdinalIgnoreCase) ||
    //         r.Performer.Contains(Busqueda, StringComparison.OrdinalIgnoreCase) ||
    //         r.Album.Contains(Busqueda, StringComparison.OrdinalIgnoreCase)
    //     ).ToList();
    //     Rolas.Clear();
    //     foreach (var cancion in filtradas){
    //         Rolas.Add(cancion);
    //     }
    // }
}

