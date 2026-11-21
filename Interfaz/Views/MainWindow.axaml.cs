using Avalonia.Controls;
using Interfaz.ViewModels;
namespace Interfaz.Views;

public partial class MainWindow : Window
{
    public MainWindow()
    {
        InitializeComponent();
	DataContext = new MainViewModel(StorageProvider);
    }
}
