## Setup

(personal) $ python3 -m venv ~/.virturalenvs/hw4_django_orm

(personal) $ source ~/.virturalenvs/hw4_django_orm/bin/activate

$ pip install Django
$ pip install psycopg2

or, just
$ pip install requirements.txt
if you know what you are doing ;-)

check to see if django is successfully installed
$ python -m django --version

(personal) $ cd 2019Spring/ece650/HW4_db/

---

## Configuration

$ django-admin startproject hw4_django_orm

in settings -> databases

```python
DATABASES = {
    'default': {
        # 'ENGINE': 'django.db.backends.sqlite3',
        # 'NAME': os.path.join(BASE_DIR, 'db.sqlite3'),
        'ENGINE': 'django.db.backends.postgresql_psycopg2',
        'NAME': 'ACC_BBALL',
        'USER':'postgres',
        'PASSWORD':'passw0rd',
        'HOST':'',
        'PORT':'',
    }
}
```
change it as above to allow postgresql to work with Django

$ python manage.py runserver 0:8000

run an http server to see if Django is running smoothly. make sure you have a external ip for your server! 

otherwise, just

$ python manage.py runserver 8000

---

## Implementation

below magic happens ;-)

$ python manage.py inspectdb

this line will help you create the Django ORM models from existing relations in db.

make sure to double check the output from inspectdb

---

## Links

urls:

- the default demo page
http://vcm-615.vm.duke.edu:8000/demo/

- the default admin backend and database manager
http://vcm-615.vm.duke.edu:8000/admin/

tests:

http://vcm-615.vm.duke.edu:8000/demo/query1/1/35/40/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/

http://vcm-615.vm.duke.edu:8000/demo/query2/Orange/

http://vcm-615.vm.duke.edu:8000/demo/query3/Duke/

http://vcm-615.vm.duke.edu:8000/demo/query4/NC/LightBlue/

http://vcm-615.vm.duke.edu:8000/demo/query5/13/

---

## References

- shorter version of Django 101 ~ easier than official tutorial
https://www.digitalocean.com/community/tutorials/how-to-use-postgresql-with-your-django-application-on-ubuntu-14-04

- static html table tags
https://www.w3schools.com/html/html_tables.asp

- url schema
https://docs.djangoproject.com/en/2.1/intro/tutorial03/

- lookuptype
https://docs.djangoproject.com/en/dev/ref/models/querysets/#lt

- always true
https://stackoverflow.com/questions/33517468/always-true-q-object

- Q objects
https://docs.djangoproject.com/en/2.1/topics/db/queries/#complex-lookups-with-q-objects

- select_related(), not too useful if you dont care about performance
https://docs.djangoproject.com/en/dev/ref/models/querysets/#select-related

- filter(), differs from get() which only returns 1 object. similar to bs4 find() and find_all()
https://docs.djangoproject.com/en/2.1/ref/models/querysets/#django.db.models.query.QuerySet.filter

- Django queries example
https://docs.djangoproject.com/en/2.1/topics/db/queries/

