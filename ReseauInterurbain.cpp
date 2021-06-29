/**
 * \file ReseauInterurbain.cpp
 * \brief Implémentattion de la classe ReseauInterurbain.
 * \author IFT-2008 & François Gauthier-Drouin
 * \version 0.1
 * \date mars 2021
 *
 *  Travail pratique numéro 2
 *
 */
#include <sstream>
#include <fstream>
#include "ReseauInterurbain.h"
//vous pouvez inclure d'autres librairies si c'est nécessaire
#include <string>
#include <stack>
#include <algorithm>
#include <vector>

namespace TP2
{

// Méthode fournie
void ReseauInterurbain::chargerReseau(std::ifstream & fichierEntree)
{
    if (!fichierEntree.is_open())
        throw std::logic_error("ReseauInterurbain::chargerReseau: Le fichier n'est pas ouvert !");

    std::string buff;

	getline(fichierEntree, nomReseau);
	nomReseau.erase(0, 20); // Enlève: Reseau Interurbain:

	int nbVilles;

	fichierEntree >> nbVilles;
	getline(fichierEntree, buff); //villes

	unReseau.resize(nbVilles);

	getline(fichierEntree, buff); //Liste des villes

	size_t i = 0;

    getline(fichierEntree, buff); //Premiere ville

    while(buff != "Liste des trajets:")
    {
        unReseau.nommer(i, buff);
        getline(fichierEntree, buff);
        i++;
    }

    while(!fichierEntree.eof())
    {
        getline(fichierEntree, buff);
		std::string source = buff;
		getline(fichierEntree, buff);
		std::string destination = buff;

		getline(fichierEntree, buff);
		std::istringstream iss(buff);

		float duree;
		iss >> duree;

		float cout;
		iss >> cout;

		unReseau.ajouterArc(unReseau.getNumeroSommet(source), unReseau.getNumeroSommet(destination), duree, cout);
	}
}

//À compléter par l'implémentation des autres méthodes demandées

    /**
    * \brief Constructeur du ReseauInterurbain
    *
    * \post Une instance du ReseauInterurbain est cree
    * \param[in] nomReseau nom du réseau
    * \param[in] nbVilles nombre de villes dans le réseau
    */
    ReseauInterurbain::ReseauInterurbain(std::string nomReseau, size_t nbVilles)
            :unReseau(nbVilles), nomReseau(nomReseau)
    {

    }

    /**
    * \brief Destructeur de ReseauInterurbain
    *
    * \post l'instance du ReseauInterurbain est supprimée
    */
    ReseauInterurbain::~ReseauInterurbain(){

    }

    /**
    * \brief Modifie le nombre de noeuds du réseau
    *
    * \post Le reseau a une taille modifiée
    * \param[in] nouvelleTaille la nouvelle taille du reseau
    */
    void ReseauInterurbain::resize(size_t nouvelleTaille){
        unReseau.resize(nouvelleTaille);
    }


    /**
    * \brief Calcule le plus court chemin entre deux sommets selon une pondération monétaire ou en temps
    *
    * \post Le réseau a une taille modifiée
    * \param[in] source sommet de départ
    * \param[in] destination sommet à atteindre
    * \param[in] dureeCout vrai si on utilise la durée comme pondération, sinon c'est faux
    */
    Chemin ReseauInterurbain::rechercheCheminDijkstra(const std::string & source, const std::string & destination, bool dureeCout) const {
        Chemin meilleurChemin;
        meilleurChemin.coutTotal = 0;
        meilleurChemin.dureeTotale= 0;
        meilleurChemin.reussi = false;

        std::vector<int> precedents(unReseau.getNombreSommets());
        std::vector<int> visite(unReseau.getNombreSommets(),false);
        std::vector<int> decouvert(unReseau.getNombreSommets(),false);
        std::vector<float> poids(unReseau.getNombreSommets(),1e6);
        std::vector<int> a_visiter;

        size_t noeudSource = unReseau.getNumeroSommet(source);
        size_t noeudActuel = noeudSource;
        size_t noeudDestination =  unReseau.getNumeroSommet(destination);
        poids[noeudActuel] = 0.;
        a_visiter.push_back(noeudActuel);
        while(a_visiter.size()>0){
            //explore les points
            for(auto fin_arc: unReseau.listerSommetsAdjacents(noeudActuel)){
                //mise a jour temptative des poids
                float distanceAParcourir = getDistanceAParcourir(noeudActuel, fin_arc, dureeCout);
                if(poids[fin_arc] > poids[noeudActuel]+distanceAParcourir){
                    poids[fin_arc]=poids[noeudActuel]+distanceAParcourir;
                    precedents[fin_arc] = noeudActuel;
                }
                if(!visite[fin_arc] and !decouvert[fin_arc]){
                    decouvert[fin_arc] = true;
                    a_visiter.push_back(fin_arc);
                }
            }

            //on trouve la prochaine ville a explorer
            a_visiter.erase(std::remove(a_visiter.begin(), a_visiter.end(), noeudActuel), a_visiter.end());
            visite[noeudActuel] = true;
            float poids_a_battre = 1e6;
            for(auto arc: a_visiter){
                if(poids[arc]<poids_a_battre){
                    poids_a_battre =poids[arc];
                    noeudActuel = arc;
                }
            }

            //si on a atteint notre but
            if(noeudActuel==noeudDestination){
                std::vector<size_t> chemin_inverse;
                chemin_inverse.push_back(noeudActuel);
                do {
                    chemin_inverse.push_back(precedents[noeudActuel]);
                    noeudActuel = precedents[noeudActuel];
                }while( noeudActuel != noeudSource );
                std::reverse(chemin_inverse.begin(),chemin_inverse.end());
                for(size_t index=0; index<chemin_inverse.size();index++){
                    if(index>0){
                        meilleurChemin.dureeTotale += getDistanceAParcourir(chemin_inverse[index-1],chemin_inverse[index],true);
                        meilleurChemin.coutTotal+= getDistanceAParcourir(chemin_inverse[index-1],chemin_inverse[index],false);

                    }
                    meilleurChemin.listeVilles.push_back(unReseau.getNomSommet(chemin_inverse[index]));
                }

                meilleurChemin.reussi = true;
                return meilleurChemin;

            }
        }
        return meilleurChemin;
    }

    float ReseauInterurbain::getDistanceAParcourir(size_t source, size_t destination, bool dureeCout) const{

        Ponderations poids = unReseau.getPonderationsArc(source,destination);
        float a_ajouter;

        if(dureeCout){
            return poids.duree;
        }else{
            return poids.cout;
        }
    }

    /**
    * \brief Retourne les composantes fortement connexes du réseau étudié selon l'algo Kosaraju
    */

    std::vector<std::vector<std::string> > ReseauInterurbain::algorithmeKosaraju(){

        std::vector<size_t> parcours_profondeur = _parcoursCompletEnProfondeur();

        std::reverse(parcours_profondeur.begin(),parcours_profondeur.end());
        std::vector<std::vector<std::string> > compos;
        std::vector<bool> visite(unReseau.getNombreSommets(),false);

        for(auto index: parcours_profondeur){
            std::vector<std::string> compFortementConnexe;

            if(!visite[index])
            {
                for(auto noeud : _parcoursEnProfondeur(index, visite,true))
                {
                    compFortementConnexe.push_back(unReseau.getNomSommet(noeud));
                }
                compos.push_back(compFortementConnexe);
            }
        }
        return compos;
    }

    std::vector<size_t> ReseauInterurbain::_parcoursCompletEnProfondeur() const
    {
        std::vector<size_t> parcours_profondeur;
        std::vector<bool> visite(unReseau.getNombreSommets(),false);
        for(size_t index=0; index < unReseau.getNombreSommets(); index++)
        {
            if(!visite[index])
            {
                for(auto noeud : _parcoursEnProfondeur(index, visite,false))
                {
                    parcours_profondeur.push_back(noeud);
                }
            }
        }
        return parcours_profondeur;
    }

    std::vector<size_t> ReseauInterurbain::_parcoursEnProfondeur(size_t source, std::vector<bool> & visite, bool reverse) const {

        std::vector<size_t> resultat;
        std::stack<size_t> pile;
        resultat = _parcours(source, pile, visite, reverse);

        return resultat;
    }

    std::vector<size_t> ReseauInterurbain::_parcours(size_t source, std::stack<size_t> & container, std::vector<bool> & visite, bool reverse) const {

        std::vector<size_t> resultat;

        // On a besoin d'une manière de marquer si un sommet a été
        // visité ou non. Utilisons un vecteur de Booléens (comme nos sommets sont
        // numérotés de 0 à m_nbSommets - 1)

        // On visite le sommet de départ et on l'ajoute comme prochain
        visite[source] = true;
        container.push(source);

        // Tant que le parcours n'est pas terminé
        while (container.size() > 0) {
            // On récupère le prochain et liste ses sommets adjacents
            size_t prochain = container.top();
            container.pop();
            resultat.push_back(prochain);

            // On liste les sommets adjacents du sommet de départ.
            std::vector<size_t> adjacents;
            if(!reverse){
                adjacents = unReseau.listerSommetsAdjacents(prochain);
            }else{
                adjacents = unReseau.listerSommetsAdjacentsInverse(prochain);
            }

            // Pour chaque sommet adjacent non visité, on l'ajoute à la pile de
            // sommets à visiter.
            for (auto it = adjacents.cbegin(); it != adjacents.cend(); ++it) {
                if (!visite[*it]) {
                    container.push(*it);
                    visite[*it] = true;
                }
            }
        }

        return resultat;
    }

}//Fin du namespace
