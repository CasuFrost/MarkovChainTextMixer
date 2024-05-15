# Markov Chain Text Mixer

## Utilizzo

Dentro la directory sorgente, sono contenute due directory, rispettivamente 
- MultiProcess 
- SingleProcess 

Dentro tali directory, sono contenuti i rispettivi eseguibili : 

- MultiProcess/multiProcess
- MultiProcess/singleProcess

Si apra la linea di comando, spostandosi nella cartella in cui è presente uno dei due eseguibili (a seconda della volontà di voler utilizzare la versione a singolo processo, oppure la versione multiprocesso).


---


### Comandi 

compito 1 : ```./singleProcess  -1 -i [input file] -o [output file]  ```

compito 2 : ``` ./singleProcess  -2 -o [output file] -i [input file] -n [numero parole] -s[parola iniziale] ```

Tutti i comandi sono obbligatori, escluso il comando ``` -s ```.

Esempio di utilizzo del programma :

```./singleProcess  -1 -i testFile/commedia.txt -o OUTPUT/CSV/commedia.csv  ```

``` ./singleProcess  -2 -o OUTPUT/randomTxt/commedia.txt  -i OUTPUT/CSV/commedia.csv -n 100000 ```

---

Spiegazione dettagliata : 

``` -1 ``` Tale comando specifica che si vuole eseguire il compito 1 del programma, ossia la conversione da file di testo, a file CSV contenente le frequenze. Tale comando non può essere usato in combinazione con il comando successivo.

``` -2 ```  Tale comando specifica che si vuole eseguire il compito 2 del programma, ossia la 
generazione casuale del testo a partire da un file CSV contenete le frequenze. Tale comando non può essere usato in combinazione con il comando precedente.

``` -i ``` Specifica il file di input che dovrà essere selezionato dal programma, se si vuole eseguire il compito 2, tale file dovrà essere un file CSV, se invece si vuole eseguire il compito 1, il file dovrebbe rappresentare un semplice file di testo.

``` -o ``` Specifica il file di output che dovrà essere generato dal programma, se si vuole eseguire il compito 2, tale file sarà essere un file di testo, se invece si vuole eseguire il compito 1, tale file sarà un file CSV. Se viene selezionato un file inesistente, verrà creato, se viene selezionato un file esistente, verrà sovrascritto. Non è possibile selezionare un percorso inesistente, la directory in cui si vuole generare il file deve esistere.

``` -n ``` Specifica il numero di parole che verranno generate nel compito 2. 

``` -s ``` Specifica la parola che verrà utilizzata come punto di partenza nel compito 2, 
è necessario che la parola sia presente nel file CSV. La parola specificata deve essere 
necessariamente minuscola.

