#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct CARD Card;

struct CARD {
    int value;
    int id;
    Card *next_card;
};

typedef struct Deck {
    int size;
    Card *cards;
} Deck;

typedef struct Hand {
    int size;
    Card *cards;
} Hand;


Card* createCard();
Deck* createDeck();
Hand* createHand();
int addCard(Card* card, Deck* deck);
int cardExists(int id, Deck* deck);
int cardExistsC(Card* card, Deck* deck);
void shuffle(Deck* deck);
int fillHand(int size, Hand* hand, Deck* deck);
void showHand(Hand* hand);
int playCard(int id, Hand* hand, Deck* deck);


int main(int argc, char** argv)
{
    // Initialise seed for random number generator
    srand(time(NULL));
    int playable = 1;

    Deck* deck = createDeck();
    Hand* player1 = createHand();
    Hand* player2 = createHand();

    if (deck == NULL || player1 == NULL || player2 == NULL)
    {
        if (deck != NULL)
            free(deck);
        if (player1 != NULL)
            free(player1);
        if (player2 != NULL)
            free(player2);
        playable = 0;
    }

    if (!playable)
    {
        return -1;
    }
    else
    {
        int i = 0;
        for (i = 0; i < 52; i++)
        {
            Card* temp = createCard();
            addCard(temp, deck);
        }

        shuffle(deck);

        fillHand(7, player1, deck);
        fillHand(7, player2, deck);

        for (i = 0; i < 7; i++)
        {
            showHand(player1);
            showHand(player2);

            playCard(-1, player1, deck);
            playCard(-1, player2, deck);
        }

        free(deck);
        free(player1);
        free(player2);
    }

    return 0;
}


// Create a new card generating a random ID (0-10,000)and value from 0-10
Card* createCard()
{
    Card* card = NULL;
    card = (Card*)malloc(sizeof(Card));
    if (card != NULL) {
        card->id = rand() % 10001;
        card->value = rand() % 11;
        card->next_card = NULL;
    }
    return card;
}

// Create an empty deck
Deck* createDeck()
{
    Deck* deck = NULL;
    deck = (Deck*)malloc(sizeof(Deck));
    if (deck != NULL) {
        deck->size = 0;
        deck->cards = NULL;
    }
    return deck;
}

// Create an empty hand
Hand* createHand()
{
    Hand* hand = NULL;
    hand = (Hand*)malloc(sizeof(Hand));
    if (hand != NULL) {
        hand->size = 0;
        hand->cards = NULL;
    }
    return hand;
}

// Add a card to the deck
int addCard(Card* card, Deck* deck)
{
    if (card == NULL || deck == NULL)
        return 0;

    if (deck->size == 0) {
        deck->cards = card;
    } else {
        Card* current_card = deck->cards;
        while (current_card->next_card != NULL) {
            current_card = current_card->next_card;
        }
        current_card->next_card = card;
    }
    deck->size++;

    return 1;
}

// Check if a card with the given ID exists in the deck
int cardExists(int id, Deck* deck)
{
    if (deck == NULL)
        return 0;

    Card* current_card = deck->cards;
    while (current_card != NULL) {
        if (current_card->id == id) {
            return 1;
        }
        current_card = current_card->next_card;
    }
    return 0;
}

// Check if a card exists in the deck (by comparing memory addresses)
int cardExistsC(Card* card, Deck* deck)
{
    if (card == NULL || deck == NULL)
        return 0;

    Card* current_card = deck->cards;
    while (current_card != NULL) {
        if (current_card == card) {
            return 1;
        }
        current_card = current_card->next_card;
    }
    return 0;
}

// Shuffle the deck by randomly rearranging the cards
void shuffle(Deck* deck)
{
    if (deck == NULL || deck->size <= 1)
        return;

    int i, j;
    Card* temp;

    for (i = deck->size - 1; i > 0; i--) {
        j = rand() % (i + 1);

        // Swap cards at index i and j
        temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
    }
}

// Fill the hand with cards from the deck
int fillHand(int size, Hand* hand, Deck* deck)
{
    if (size <= 0 || hand == NULL || deck == NULL)
        return 0;

    int i;
    Card* current_card;
    for (i = 0; i < size; i++) {
        if (deck->size == 0)
            break;

        current_card = deck->cards;
        hand->cards = (Card*)realloc(hand->cards, (hand->size + 1) * sizeof(Card));
        if (hand->cards == NULL)
            return 0;

        hand->cards[hand->size] = current_card;
        hand->size++;

        deck->cards = deck->cards->next_card;
        deck->size--;
    }

    return 1;
}

// Display the cards in the hand
void showHand(Hand* hand)
{
    if (hand == NULL || hand->size == 0) {
        printf("Hand is empty\n");
        return;
    }

    printf("Hand size: %d\n", hand->size);
    int i;
    for (i = 0; i < hand->size; i++) {
        printf("Card %d: ID=%d, Value=%d\n", i+1, hand->cards[i]->id, hand->cards[i]->value);
    }
    printf("\n");
}

// Play a card from the hand by removing it and adding it back to the deck
int playCard(int id, Hand* hand, Deck* deck)
{
    if (hand == NULL || deck == NULL)
        return 0;

    int i, index = -1;
    for (i = 0; i < hand->size; i++) {
        if (id == -1 || hand->cards[i]->id == id) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        Card* card = hand->cards[index];
        if (cardExistsC(card, hand) && !cardExistsC(card, deck)) {
            hand->size--;
            for (i = index; i < hand->size; i++) {
                hand->cards[i] = hand->cards[i + 1];
            }
            hand->cards = (Card*)realloc(hand->cards, hand->size * sizeof(Card));

            addCard(card, deck);
            return 1;
        }
    }
    return 0;
}