describe('Blog app', function() {
  beforeEach(function() {
    cy.request('POST', 'http://localhost:3003/api/testing/reset')
    cy.request('POST', 'http://localhost:3003/api/users', {
      username: 'Joku1',
      name: 'Jotakin3',
      password: 'Lokki6'
  })
    cy.visit('http://localhost:3000')
  })

  it('Login form is shown', function() {
    cy.contains('Login to application')
  })
  describe('Login',function() {
    it('succeeds with correct credentials', function() {
      cy.contains('username').find('input').type('Joku1')
      cy.contains('password').find('input').type('Lokki6')
      cy.contains('login').click()

      cy.contains('Jotakin3 Logged in')
    })

    it('fails with wrong credentials', function() {
      cy.contains('username').find('input').type('Joku1')
      cy.contains('password').find('input').type('Lokki7')
      cy.contains('login').click()

      cy.contains('wrong username or password').should('have.css', 'color', 'rgb(255, 0, 0)')
    })
  })
  describe('When logged in', function() {
    beforeEach(function() {
      cy.request('POST', 'http://localhost:3003/api/login', {
      username: 'Joku1', password: 'Lokki6'
    }).then(response => {
      localStorage.setItem('loggedBlogappUser', JSON.stringify(response.body))
      cy.visit('http://localhost:3000')
    })
    })

    it('A blog can be created', function() {
      cy.contains('create new blog').click()
      cy.get('#blogForm').contains('title').find('input').type('testTitle')
      cy.get('#blogForm').contains('author').find('input').type('testAuthor')
      cy.get('#blogForm').contains('url').find('input').type('testUrl')
      cy.get('#blogForm').find('button').click()
      cy.contains('testTitle testAuthor')
    })
    describe('one blog added', function() {
      beforeEach(function() {
        cy.request({
          method: 'POST',
          url: 'http://localhost:3003/api/blogs',
          body: {
          title: 'testi1', author: 'testi2', url: 'testiOsoite' },
          headers: {
            'Authorization': `bearer ${JSON.parse(localStorage.getItem('loggedBlogappUser')).token}`
          }
        }).then(response => {
      cy.visit('http://localhost:3000')
    })
      })
      it('a blog can be liked', function() {
        cy.contains('view').click()
        cy.contains('likes 0').find('button').click()
        cy.contains('likes 1').find('button').click()
        cy.contains('likes 2')
      })
      it('blog can be removed by the user who created it', function() {
        cy.contains('view').click()
        cy.contains('remove').click()
        cy.contains('blog testi1 by testi2 deleted')
        cy.get('html').should('not.contain', 'testiOsoite')
      })

      describe('another blog added', function() {
        beforeEach(function() {
          cy.request({
            method: 'POST',
            url: 'http://localhost:3003/api/blogs',
            body: {
            title: 'testi3', author: 'testi4', url: 'testiOsoite2' },
            headers: {
              'Authorization': `bearer ${JSON.parse(localStorage.getItem('loggedBlogappUser')).token}`
            }
          }).then(response => {
        cy.visit('http://localhost:3000')
      })
    })
        it('blogs ordered by likes', function () {
          cy.get('.blog').eq(0).should('contain', 'testi1 testi2')
          cy.get('.blog').eq(1).should('contain', 'testi3 testi4')

          cy.get('.blog').eq(1).contains('view').click()
          cy.get('.blog').eq(1).contains('like').click()

          cy.get('.blog').eq(1).should('contain', 'testi1 testi2')
          cy.get('.blog').eq(0).should('contain', 'testi3 testi4')
        })
    })

  })

})
})