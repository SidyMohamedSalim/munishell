!/bin/bash

echo "Bienvenue sur le générateur de mots de passe"
echo "Veuillez entrer la taille de votre mot de passe : "
read -r LEN

# Vérification que l'entrée est un nombre positif
if ! [[ "$LEN" =~ ^[0-9]+$ ]] || [ "$LEN" -le 0 ]; then
    echo "Erreur : Veuillez entrer un nombre entier positif"
    exit 1
fi

echo "Génération de 5 mots de passe de $LEN caractères :"
echo "================================================"

for p in $(seq 1 5); do
    # Génération d'un mot de passe sécurisé
    password=$(openssl rand -base64 48 | tr -d "=+/" | cut -c1-"$LEN")
    echo "Mot de passe $p : $password"
done

echo "================================================"
echo "Génération terminée !"