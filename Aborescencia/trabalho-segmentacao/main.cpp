#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "Image.h"
#include "utils.h"
#include "Segmenter.h"
#include "GraphBuilder.h"
#include "MST.h"
#include "Arborescence.h"

static std::string add_suffix(const std::string& filename, const std::string& suffix) {
    size_t dot = filename.find_last_of('.');
    if (dot == std::string::npos) return filename + suffix;
    return filename.substr(0, dot) + suffix + filename.substr(dot);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0]
            << " entrada.png saida.png\n";
        return 1;
    }

    std::string entrada = argv[1];
    std::string saida_base = argv[2];

    // Numero de segmentos
    int num_segments;
    std::cout << "Digite o numero de segmentos desejado: ";
    std::cin >> num_segments;

    if (num_segments < 1) {
        std::cout << "Valor invalido. Usando 1.\n";
        num_segments = 1;
    }

    std::vector<double> intensities;
    Image rawImg = loadImage(entrada, intensities);

    if (rawImg.width == 0) {
        std::cerr << "Erro ao carregar imagem.\n";
        return 1;
    }

    //pre-processamento
    std::cout << "Aplicando Gaussian Blur (sigma=0.8)...\n";
    Image img = applyGaussianBlur(rawImg, 0.8);

    int n = img.width * img.height;
    std::cout << "Imagem carregada: " << img.width << " x " << img.height
        << " (" << n << " pixels)\n";

    /*
    MENU DE ESCOLHA DE MÉTODO (Ignorado para forçar Arborescência)

    std::cout << "\nEscolha o metodo:\n";
    std::cout << "1 - Apenas MST (Kruskal)\n";
    std::cout << "2 - Apenas Arborescencia Minima (via MST orientada)\n";
    std::cout << "3 - Ambos\n";
    std::cout << "Opcao: ";

    int opcao = 1;
    std::cin >> opcao;
    if (opcao < 1 || opcao > 3) opcao = 1;
    */
    
    // Definimos opção fixa ou ignoramos a variável opcao, 
    // pois vamos forçar a execução da Arborescência abaixo.

    double cost_mst = 0.0;
    double cost_arb = 0.0;

    // Rodar MST
    auto run_mst = [&](const std::string& out_name) -> bool {
        auto graph = buildGraph(img);
        auto mst = kruskal_mst(n, graph);

        cost_mst = mst.first;
        std::cout << "[MST] Custo total: " << cost_mst << "\n";

        auto labels = segment_by_mst(n, mst.second, num_segments);
        Image seg = labels_to_color_image(img.width, img.height, labels);

        if (!saveImagePNG(seg, out_name)) {
            std::cerr << "[MST] Falha ao salvar imagem.\n";
            return false;
        }
        std::cout << "[MST] Imagem salva em: " << out_name << "\n";
        return true;
    };

    // Rodar Arborescencia
    auto run_arbo = [&](const std::string& out_name) -> bool {
        auto graph = buildGraph(img);
        
        auto mst = kruskal_mst(n, graph); 

        int root = 0;
        auto arb = arborescence_from_mst(n, mst.second, root);

        cost_arb = arb.first;
        std::cout << "[ARBO] (via MST orientada) Custo total: " << cost_arb << "\n";

        auto labels = segment_by_mst(n, arb.second, num_segments);
        Image seg = labels_to_color_image(img.width, img.height, labels);

        if (!saveImagePNG(seg, out_name)) {
            std::cerr << "[ARBO] Falha ao salvar imagem.\n";
            return false;
        }
        std::cout << "[ARBO] Imagem salva em: " << out_name << "\n";
        return true;
    };

    bool ok = true;

    //EXECUÇÃO APENAS DA ARBORESCÊNCIA
    std::cout << "\nExecutando apenas Metodo de Arborescencia...\n";
    ok = run_arbo(saida_base);

    /*
    LOGICA CONDICIONAL E COMPARAÇÃO

    if (opcao == 1) {
        ok = run_mst(saida_base);
    }
    else if (opcao == 2) {
        ok = run_arbo(saida_base);
    }
    else {
        std::string out_mst = add_suffix(saida_base, "_mst");
        std::string out_arbo = add_suffix(saida_base, "_arbo");

        ok = run_mst(out_mst) && run_arbo(out_arbo);

        if (ok) {
            std::cout << "\n<<< COMPARACAO ENTRE METODOS >>>\n";
            std::cout << "MST (Kruskal):              " << cost_mst << "\n";
            std::cout << "Arborescencia (MST orient): " << cost_arb << "\n";
            std::cout << "Diferenca absoluta:         "
                << std::fabs(cost_mst - cost_arb) << "\n";

            if (cost_mst < cost_arb)
                std::cout << "Metodo mais barato:         MST\n";
            else if (cost_mst > cost_arb)
                std::cout << "Metodo mais barato:         Arborescencia\n";
            else
                std::cout << "Os dois metodos empataram!\n";
        }
    }
    */

    if (!ok) return 1;

    std::cout << "Fim OK.\n";
    return 0;
}