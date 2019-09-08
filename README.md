# Warning
Original version was lost, this is some backup which doesn't work, but the code is there.
I think that update of Qt framework has something to do with it

# RS014-risk
![nema slike](https://github.com/mi15405/Risk/blob/master/screenshots/RiskScreenshot.PNG)
### Osnovno o projektu

Društvena igra bazirana na diplomatiji i osvajanju za 2 do 6 igrača. Svaki igrač ima teritorije koje su pod</br> njegovom kontrolom, vojsku, resurse i kartice koje je sakupio. Na svakoj teritorije moze da sagradi određena</br> utvrđenja koja bi mu pomogla u daljem osvajanju. Igra se sa 5 kockica (sa vrednostima 1 - 10). U toku igre su</br> mogući i slučajni događaji, koji mogu igraču na potezu da pomognu ili odmognu (gubljenje vojske na nekoj</br> teritoriji, propuštanje poteza, povećanje faktora sreće...).


### Pravila:
- Svaki igrač dobija nasumično dodeljene teritorije, i postavlja vojsku gde želi
- Igrač koji je na potezu:
	- dobija dodatnu vojsku u zavisnosti od osvojenih teritorija (prvi potez se ne računa) i osvojenih kontinenata
	- dobija resurse u zavisnosti od teritorija koje poseduje (prvi potez se ne računa)
	- sa malom verovatnoćom se pokreće slučajni događaj
	- sakupljene kartice, ukoliko to želi, moze da zameni za vojsku
	- može da iskoristi resurse, i sagradi neko utvrđenje
	- kreće sa napadima (ima na raspolaganju neograničen broj napada)
	- ukoliko ima 24 teritorije - pobedio je, a igra se završava
	- kad odluči da je završio sa napadanjem, može da pregrupiše trupe **sa jedne teritorije** na susednu
	- ukoliko je u toku poteza osvojio **bar jednu** teritoriju, dobija jednu karticu iz špila

### Komponente:
1. Implementacija grafičkog korisničkog interfejsa uz pomoć Qt biblioteke za GUI (mapa sveta, grafika za bacanje kockica, interakcija za poteze)
2. Implementacija logičkog dela (pravila igre, potezi, nagrade... )
3. _DODATNO:_ &nbsp; izmena klasičnog rizika (resursi, efekti ... )

### Plan:
- [ ] Detaljno razraditi logiku igre
- [ ] Napraviti mapu sveta, i ostatak grafičkog korisničkog interfejsa
- [ ] Implementirati osnovne funkcionalnosti igre (podela teritorija, napadanje, prepoznavanje klika na teritoriju)
