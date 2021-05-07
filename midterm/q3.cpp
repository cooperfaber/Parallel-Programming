


void foo(int *a, int *b, int *c, mutex *m0, mutex *m1) { 
    //no c in this function, so only m0 is needed
       if (b[0] > 0) { 
            m0.lock();
            a[0] = b[0];
            m0.unlock();   
        }    
       else if (b[0] > -16) { 
            m0.lock();       
            a[0] = 128;
            m0.unlock(); 
            return;    
       }
        else {
            m0.lock();          
            a[0] = 256;
            m0.unlock();    
        }
    m0.lock();  
    a[0]++;
    m0.unlock(); 
}


void bar(int *a, int *b, int *c, mutex *m0, mutex *m1) {
        //as only c is passed to this function, no m0 is needed
        m1.lock();
        c[0] = a[0];
        m1.unlock();
    }

void baz(int *a, int *b, int *c, mutex *m0, mutex *m1) {
        //as b is more or less read-only, only m0 is used
        m0.lock();
        a[0] = b[0];
        m0.unlock();
    }

void qux(int *a, int *b, int *c, mutex *m0, mutex *m1) {
    //no a, so only m1
        m1.lock();
        c[0] = b[0] + c[0];
        m1.unlock();
    }

int main() 
{
    int*a = newint[1];
    int*b = newint[1];
    int*c = newint[1];
    mutexm0, m1;
    threadtfoo = thread(foo, a, b, c, &m0, &m1);
    threadtbar = thread(bar, c, c, c, &m0, &m1);
    threadtbaz = thread(baz, a, b, c, &m1, &m0);
    threadtqux = thread(qux, a, b, c, &m0, &m1);
    // assume we join and cleanup memory after this
}