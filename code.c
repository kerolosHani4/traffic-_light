signed char i = 0, flag = 0, count = 0;
void counter(signed char i)
{
    portc = 0;
    portb = (i / 10) + (i % 10) * 16;
    delay_ms(500);
}
void toggle(void)
{
    if (porta.b2 == 0)
    {
        flag = ~flag;
        delay_ms(500);
    }
}
void west(void)
{
    portd.b0 = 1;
    for (i = 15;; i--)
    {
        if (porta.b1 == 0)
            break;
        if (i < 0)
        {
            portd.b4 = 0;
            break;
        }
        if (i <= 3)
        {
            portd.b5 = 0;
            portd.b4 = 1;
        }
        else
            portd.b5 = 1;
        counter(i);
    }
    portd.b0 = 0;
    portd.b5 = 0;
}
void south(void)
{
    portd.b3 = 1;
    for (i = 23; i >= 0; i--)
    {
        if (porta.b1 == 0)
            break;
        if (i < 0)
        {
            portd.b1 = 0;
            break;
        }
        if (i <= 3)
        {
            portd.b1 = 1;
            portd.b2 = 0;
        }
        else
            portd.b2 = 1;
        counter(i);
    }
    portd.b2 = 0;
    portd.b3 = 0;
}
void automatic(void)
{
    while (porta.b1 == 1)
    {
        portb = 0x00;
        portd = 0x00;
        west();
        south();
    }
}
void manual(void)
{
    while (porta.b1 == 0)
    {
        portd = 0x00;
        portb = 0x00;
        portc = 15;
        while (flag == 0)
        {
            portd.b2 = 0;
            portd.b3 = 0;
            portd.b5 = 1;
            for (i = 0; i < 3; i++)
            {
                portd.b1 = 1;
                delay_ms(500);
                portd.b1 = 0;
                delay_ms(500);

                toggle();
                if (flag == 1)
                    break;
                if (porta.b1)
                    break;
            }
            while (1)
            {
                portd.b0 = 1;

                toggle();
                if (flag)
                    break;
                if (porta.b1)
                    break;
            }

            if (porta.b1)
                break;
        }
        while (flag)
        {
            portd.b0 = 0;
            portd.b5 = 0;

            portd.b2 = 1;
            for (i = 0; i < 3; i++)
            {
                portd.b4 = 1;
                delay_ms(500);
                portd.b4 = 0;
                delay_ms(500);

                toggle();
                if (flag == 0)
                    break;
                if (porta.b1)
                    break;
            }
            while (1)
            {
                portd.b3 = 1;

                toggle();
                if (flag == 0)
                    break;
                if (porta.b1)
                    break;
            }

            if (porta.b1)
                break;
        }
    }
}

void main()
{
    adcon1 = 0x07;
    trisb = 0x00;
    portb = 0x00;
    trisc = 0x00;
    portc = 15;
    trisd = 0x00;
    portd = 0x00;
    trisa = 7;
    while (porta.b0)
        ;
    while (1)
    {
        automatic();
        manual();
    }
}