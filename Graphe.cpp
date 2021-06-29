/**
 * \file Graphe.cpp
 * \brief Implémentation d'un graphe orienté.
 * \author IFT-2008 & François Gauthier-Drouin
 * \version 0.1
 * \date mars 2021
 *
 *  Travail pratique numéro 2
 *
 */

#include "Graphe.h"
#include "algorithm"
//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2 {
    /**
    * \brief Constructeur du graph
    *
    * \post Une instance du graphe est cree
    * \param[in] nbSommets le nombre de sommets du graphe
    */
    Graphe::Graphe(size_t nbSommets)
            : noms(nbSommets), listesAdj(nbSommets), nbSommets(nbSommets), nbArcs() {

    }

    /**
    * \brief destructeur du graph
    *
    * \post Le graph est supprimé
    */
    Graphe::~Graphe() {}


    /**
    * \brief Modifie le nombre de sommets du graphe
    *
    * \post Le nombre de sommets du graphe est modifié
    * \param[in] nouvelleTaille le nouveau nombre de sommets du graphe
    */
    void Graphe::resize(size_t nouvelleTaille) {
        noms.resize(nouvelleTaille);
        listesAdj.resize(nouvelleTaille);
        nbSommets = nouvelleTaille;
    }

    /**
     * \brief Nomme un sommet
     *
     * \post Le sommet est nommé
     * \param[in] sommet le sommet dont le nom est à modifier
     * \param[in] le nom qui est donné au sommet
     */
    void Graphe::nommer(size_t sommet, const std::string &nom) {
        if (sommet <= nbSommets) {
            noms[sommet] = nom;
        } else {
            throw std::logic_error("index du sommet invalide");
        }
    }

    /**
     * \brief Rajoute un arc entre deux sommets au graphe
     *
     * \post Le nouvel arc est ajouté
     * \param[in] source point de depart
     * \param[in] destination point d'arrivée
     * \param[in] duree durée de l'arc en temps
     * \param[in] cout coût monétaire de parcourir l'arc
     */
    void Graphe::ajouterArc(size_t source, size_t destination, float duree, float cout)
    {
        if (source <= nbSommets and destination <= nbSommets)
        {
            if (arcExiste(source,destination))
            {
                throw std::logic_error("Cet arc est déjà existant");
            }
                Ponderations ponderation_de_arc(duree, cout);
                Arc arc_a_ajouter(destination, ponderation_de_arc);
                arc_a_ajouter.poids = ponderation_de_arc;
                listesAdj[source].push_back(arc_a_ajouter);
                nbArcs++;
        }
        else
        {
            throw std::logic_error("index de la source ou de la destination invalide");
        }
    }


    /**
     * \brief Enleve un arc au graphe
     *
     * \post L'arc est enlevé
     * \param[in] source point de depart
     * \param[in] destination point d'arrivee
     */
    void Graphe::enleverArc(size_t source, size_t destination) {
        if (arcExiste(source, destination))
        {
            if (source <= nbSommets and destination <= nbSommets)
            {
                std::list<Arc>::iterator arc_a_enlever;
                for (std::list<Arc>::iterator i = listesAdj[source].begin(); i != listesAdj[source].end(); i++)
                {
                    if (i->destination == destination)
                    {
                        arc_a_enlever = i;
                    }
                }
                listesAdj[source].erase(arc_a_enlever);
            }
            else
            {
                throw std::logic_error("index de la source ou de la destination invalide");
            }
        }
        else
        {
            throw std::logic_error("l'arc est inexistant");
        }
    }

    /**
     * \brief Retourne si l'arc existe ou non
     *
     * \param[in] source le point de depart
     * \param[in] destination le point d'arrivee
     */
    bool Graphe::arcExiste(size_t source, size_t destination) const {
        if (source <= nbSommets and destination <= nbSommets) {
            for (auto arc: listesAdj[source]) {
                if (arc.destination == destination) {
                    return true;
                }
            }
            return false;
        }
        else
        {
            throw std::logic_error("index de la source ou de la destination invalide");
        }
    }


    /**
     * \brief Retourne la liste des sommets adjacents d'un sommet
     *
     * \param[in] source le point de depart
     */
    std::vector<size_t> Graphe::listerSommetsAdjacents(size_t sommet) const {
        if (sommet <= nbSommets) {
            std::vector<size_t> liste_adj;
            for (auto arc: listesAdj[sommet]) {
                liste_adj.push_back(arc.destination);
            }
            return liste_adj;
        }
        else
        {
            throw std::logic_error("index de la source ou de la destination invalide");
        }
    }

    /**
     * \brief Retourne le nom d'un sommet
     *
     * \param[in] size_t l'index du sommet
     */
    std::string Graphe::getNomSommet(size_t sommet) const {
        if (sommet <= nbSommets) {
            return noms[sommet];
        }
        throw std::logic_error("index du sommet invalide");
    }


    /**
     * \brief Retourne le nombre de sommet du graphe
     *
    */
    int Graphe::getNombreSommets() const {
        return nbSommets;
    }

    /**
    * \brief Retourne le nombre d'arcs dans le graphe
    *
    */
    //
    int Graphe::getNombreArcs() const {
        return nbArcs;
    }


    /**
     * \brief Retourne l'index du sommet
     *
     * \param[in] size_t nom du sommet
     */
    size_t Graphe::getNumeroSommet(const std::string &nom) const
    {
        for (size_t i = 0; i < noms.size(); i++)
        {
            if (noms[i] == nom)
            {
                return i;
            }
        }
        throw std::logic_error("le sommet est inexistant!");
    }

    /**
     * \brief Pondère le coût d'un arc
     *
     * \param[in] source point de depart
     * \param[in] destination point d'arrivée
     */
    Ponderations Graphe::getPonderationsArc(size_t source, size_t destination) const {
        if (source <= nbSommets and destination <= nbSommets)
        {
            if (arcExiste(source, destination))
            {
                for (auto arc: listesAdj[source]) {
                    if (arc.destination == destination) {
                        return arc.poids;
                    }
                }
            }
            else
            {
                throw std::logic_error("l'arc n'existe pas");
            }
        }
        else
        {
            throw std::logic_error("l'index du sommet est invalide");
        }
    }

    std::vector<size_t> Graphe::listerSommetsAdjacentsInverse(size_t sommet) const{
        std::vector<size_t> adj_inverse;
        for(size_t numero_sommet=0; numero_sommet < listesAdj.size(); numero_sommet++){
            for(auto arc: listesAdj[numero_sommet]){
                if(arc.destination==sommet){
                    adj_inverse.push_back(numero_sommet);
                }
            }
        }
        return adj_inverse;
    }

}//Fin du namespace
