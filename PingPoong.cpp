
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>

int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));

	//Constantes
	const float pi = 3.14159f;
	const int Largeur =800;
	const int Hauteur =600;
	sf::Vector2f TailleRaquette(25, 100);
	sf::Vector2f TailleLigne(5, Hauteur);
	float TailleBalle = 15.f;


	// Charger Police
	sf::Font font;
	if (!font.loadFromFile("ocrastd.otf"))
		return EXIT_FAILURE;

	// Message Pause
	sf::Text MessagePause;
	MessagePause.setFont(font);
	MessagePause.setCharacterSize(20);
	MessagePause.setPosition(170.f, 150.f);
	MessagePause.setColor(sf::Color::White);
	MessagePause.setString("Ping Pong!\nEspace pour lancer le jeu");

	// Fenetre
	sf::RenderWindow window(sf::VideoMode(Largeur, Hauteur, 32), "", sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	//Ligne Centrale
	sf::RectangleShape Ligne;
	Ligne.setSize(TailleLigne);
	Ligne.setFillColor(sf::Color::White);
	Ligne.setOrigin(TailleRaquette / 2.f);

	// Raquette gauche
	sf::RectangleShape RaquetteGauche;
	RaquetteGauche.setSize(TailleRaquette - sf::Vector2f(3, 3));
	RaquetteGauche.setOutlineThickness(3);
	RaquetteGauche.setOutlineColor(sf::Color::Black);
	RaquetteGauche.setFillColor(sf::Color(100, 100, 200));
	RaquetteGauche.setOrigin(TailleRaquette / 2.f);

	// Raquettes droites
	sf::RectangleShape RaquetteDroite;
	RaquetteDroite.setSize(TailleRaquette - sf::Vector2f(3, 3));
	RaquetteDroite.setOutlineThickness(3);
	RaquetteDroite.setOutlineColor(sf::Color::Black);
	RaquetteDroite.setFillColor(sf::Color(200, 100, 100));
	RaquetteDroite.setOrigin(TailleRaquette / 2.f);


	//Balle
	sf::CircleShape balle;
	balle.setRadius(TailleBalle  - 3);
	balle.setOutlineThickness(3);
	balle.setOutlineColor(sf::Color::Black);
	balle.setFillColor(sf::Color::White);
	balle.setOrigin(TailleBalle / 2, TailleBalle / 2);
	
	// Definie les parametre de la raquette
	const float VitesseRaq = 400.f;
	const float VitesseBalle = 400.f;
	float AngleBalle = 0; 

	sf::Clock clock;
	bool Marche = false;
	while (window.isOpen())
	{
		// Handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Fermeture
			if ((event.type == sf::Event::Closed) ||
				((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
			{
				window.close();
				break;
			}

			// Lancement Jeu
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
			{
				if (!Marche)
				{
					// (re)start the game
					Marche = true;
					clock.restart();

					//Reset
					RaquetteGauche.setPosition(10 + TailleRaquette.x / 2, Hauteur / 2 );
					RaquetteDroite.setPosition(Largeur - 10 - TailleRaquette.x / 2, Hauteur / 2);
					balle.setPosition(Largeur / 2, Hauteur / 2);
					Ligne.setPosition((Largeur+TailleLigne.x) / 2, 50);

					// Reset angle de la balle
					do
					{
						// Evite des angles trop vertical
						AngleBalle = (std::rand() % 360) * 2 * pi / 360;
					} while (std::abs(std::cos(AngleBalle)) < 0.7f);
				}
			}
		}

		if (Marche)
		{
			float delta = clock.restart().asSeconds();

			// Mouvement Joueur1
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && (RaquetteGauche.getPosition().y - TailleRaquette.y / 2 > 5.f))
			{
				RaquetteGauche.move(0.f, -VitesseRaq * delta);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && (RaquetteGauche.getPosition().y + TailleRaquette.y / 2 < Hauteur - 5.f))
			{
				RaquetteGauche.move(0.f, VitesseRaq * delta);
			}
			//Mouvement joueur 2
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && (RaquetteDroite.getPosition().y - TailleRaquette.y / 2 > 5.f))
			{
				RaquetteDroite.move(0.f, -VitesseRaq * delta);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && (RaquetteDroite.getPosition().y + TailleRaquette.y / 2 < Hauteur - 5.f))
			{
				RaquetteDroite.move(0.f, VitesseRaq * delta);
			}
			// Mouvement de la balle
			float factor = VitesseBalle * delta;
			balle.move(std::cos(AngleBalle) * factor, std::sin(AngleBalle) * factor);

			// Collision Manager
			if (balle.getPosition().x - TailleBalle < 0.f)
			{
				Marche = false;
				MessagePause.setString("Perdu!!!!\nEspace pour recommencer or\nEscape pour Quitter");
			}
			if (balle.getPosition().x + TailleBalle> Largeur)
			{
				Marche = false;
				MessagePause.setString("Gagné!\n Espace pour recommencer or\nEscape pour Quitter");
			}
			if (balle.getPosition().y - TailleBalle < 0.f)
			{
				AngleBalle = -AngleBalle;
				balle.setPosition(balle.getPosition().x, TailleBalle + 0.1f);
			}
			if (balle.getPosition().y + TailleBalle > Hauteur)
			{
				AngleBalle = -AngleBalle;
				balle.setPosition(balle.getPosition().x, Hauteur - TailleBalle - 0.1f);
			}

			// RaquetteJoueur1
			if (balle.getPosition().x - TailleBalle < RaquetteGauche.getPosition().x + TailleRaquette.x / 2 && balle.getPosition().x - TailleBalle > RaquetteGauche.getPosition().x && balle.getPosition().y + TailleBalle >= RaquetteGauche.getPosition().y - TailleRaquette.y / 2 && balle.getPosition().y - TailleBalle <= RaquetteGauche.getPosition().y + TailleRaquette.y / 2)
			{
				if (balle.getPosition().y > RaquetteGauche.getPosition().y)
					AngleBalle = pi - AngleBalle + (std::rand() % 20) * pi / 180;
				else
					AngleBalle = pi - AngleBalle - (std::rand() % 20) * pi / 180;
				balle.setPosition(RaquetteGauche.getPosition().x + TailleBalle + TailleRaquette.x / 2 + 0.1f, balle.getPosition().y);
			}

			// RaquetteJoueur2
			if (balle.getPosition().x + TailleBalle >RaquetteDroite.getPosition().x - TailleRaquette.x / 2 && balle.getPosition().x + TailleBalle < RaquetteDroite.getPosition().x && balle.getPosition().y + TailleBalle >= RaquetteDroite.getPosition().y - TailleRaquette.y / 2 && balle.getPosition().y - TailleBalle <= RaquetteDroite.getPosition().y + TailleRaquette.y / 2)
			{
				if (balle.getPosition().y > RaquetteDroite.getPosition().y)
					AngleBalle = pi - AngleBalle + (std::rand() % 20) * pi / 180;
				else
					AngleBalle = pi - AngleBalle - (std::rand() % 20) * pi / 180;
				balle.setPosition(RaquetteDroite.getPosition().x - TailleBalle - TailleRaquette.x / 2 - 0.1f, balle.getPosition().y);
			}
		}

		// Clear the window
		window.clear(sf::Color::Black);

		if (Marche)
		{
			// Dessine les raquettes/balle
			window.draw(RaquetteGauche);
			window.draw(RaquetteDroite);
			window.draw(balle);
			window.draw(Ligne);
		}
		else
		{
			// Message de pause
			window.draw(MessagePause);
		}

		// Display things on screen
		window.display();
	}

	return 0;
}
